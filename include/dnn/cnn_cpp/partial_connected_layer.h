
#pragma once
#include "util.h"
#include "layer.h"


class partial_connected_layer : public layer {
public:
    typedef std::vector<std::pair<unsigned short, unsigned short> > io_connections;
    typedef std::vector<std::pair<unsigned short, unsigned short> > wi_connections;
    typedef std::vector<std::pair<unsigned short, unsigned short> > wo_connections;
    typedef layer Base;
    //typedef typename Base::Optimizer Optimizer;

    partial_connected_layer(int in_dim, int out_dim, int weight_dim, int bias_dim, float_t scale_factor = 1.0)
        : layer(in_dim, out_dim, weight_dim, bias_dim), 
        weight2io_(weight_dim), out2wi_(out_dim), in2wo_(in_dim), bias2out_(bias_dim), out2bias_(out_dim), scale_factor_(scale_factor) {
        if (in_dim <= 0 || weight_dim <= 0 || weight_dim <= 0 || bias_dim <= 0)
            throw nn_error("invalid layer size");
    }

    int param_size() const { 
        int total_param = 0;
        for (int w=0; w<weight2io_.size(); ++w)
            if (weight2io_[w].size() > 0) total_param++;
        for (int b=0; b<bias2out_.size(); ++b)
            if (bias2out_[b].size() > 0) total_param++;
        return total_param;
    }

    int connection_size() const {
        int total_size = 0;
        for (int io=0; io<weight2io_.size(); ++io)
            total_size += weight2io_[io].size();
        for (int b=0; b<bias2out_.size(); ++b)
            total_size += bias2out_[b].size();
        return total_size;
    }

    int fan_in_size() const {
        return out2wi_[0].size();
    }

    void connect_weight(int input_index, int output_index, int weight_index) {
        weight2io_[weight_index].push_back(std::make_pair(input_index, output_index));
        out2wi_[output_index].push_back(std::make_pair(weight_index, input_index));
        in2wo_[input_index].push_back(std::make_pair(weight_index, output_index));
    }

    void connect_bias(int bias_index, int output_index) {
        out2bias_[output_index] = bias_index;
        bias2out_[bias_index].push_back(output_index);
    }

    virtual const vec_t& forward_propagation(const vec_t& in, int index) {
      
      for (int i = 0; i < this->out_size_; i++) {
        const wi_connections& connections = out2wi_[i];
        float_t a = 0.0;
        
        for (wi_connections::const_iterator connection = connections.begin(); 
        connection!=connections.end(); ++connection)// 13.1%
          a += this->W_[connection->first] * in[connection->second]; // 3.2%
        
        a *= scale_factor_;
        a += this->b_[out2bias_[i]];
        this->output_[index][i] = this->a_.f(a); // 9.6%
      }
      
      return this->next_ ? this->next_->forward_propagation(this->output_[index], index) : this->output_[index]; // 15.6%
    }

    virtual const vec_t& back_propagation(const vec_t& current_delta, int index) {
      const vec_t& prev_out = this->prev_->output(index);
      const Activation& prev_h = this->prev_->activation_function();
      vec_t& prev_delta = this->prev_delta_[index];
      int i;
      for (i = 0; i != this->in_size_; i++) {
        const wo_connections& connections = in2wo_[i];
        float_t delta = 0.0;
        
        for (wo_connections::const_iterator connection = connections.begin();
        connection!=connections.end(); ++connection) 
          delta += this->W_[connection->first] * current_delta[connection->second]; // 40.6%
        
        prev_delta[i] = delta * scale_factor_ * prev_h.df(prev_out[i]); // 2.1%
      }
      
      for (i = 0; i < weight2io_.size(); i++) {
        const io_connections& connections = weight2io_[i];
        float_t diff = 0.0;
        
        for (wo_connections::const_iterator connection = connections.begin();
        connection!=connections.end(); ++connection)  // 11.9%
          diff += prev_out[connection->first] * current_delta[connection->second];
        
        this->dW_[index][i] += diff * scale_factor_;
      }
      
      for (i = 0; i < bias2out_.size(); i++) {
        const std::vector<int>& outs = bias2out_[i];
        float_t diff = 0.0;
        
        for (const int* o= outs.begin(); o!=outs.end(); ++o)
          diff += current_delta[*o];
        
        this->db_[index][i] += diff;
      } 
      
      return this->prev_->back_propagation(this->prev_delta_[index], index);
    }

    const vec_t& back_propagation_2nd(const vec_t& current_delta2) {
        const vec_t& prev_out = this->prev_->output(0);
        const Activation& prev_h = this->prev_->activation_function();

        for (size_t i = 0; i < weight2io_.size(); i++) {
            const io_connections& connections = weight2io_[i];
            float_t diff = 0.0;

            for (wo_connections::const_iterator connection = connections.begin();
              connection!=connections.end(); ++connection)
                diff += prev_out[connection->first] * prev_out[connection->first] * current_delta2[connection->second];

            diff *= scale_factor_ * scale_factor_;
            this->Whessian_[i] += diff;
        }

        for (i = 0; i < bias2out_.size(); i++) {
            const std::vector<int>& outs = bias2out_[i];
            float_t diff = 0.0;

            for (const int* o = outs.begin(); o!=outs.end(); ++o)
                diff += current_delta2[*o];

            this->bhessian_[i] += diff;
        }

        for (i = 0; i < this->in_size_; i++) {
            const wo_connections& connections = in2wo_[i];
            this->prev_delta2_[i] = 0.0;

            for (wo_connections::const_iterator connection = connections.begin();
              connection!=connections.end(); ++connection)
                this->prev_delta2_[i] += this->W_[connection->first] * this->W_[connection->first] * current_delta2[connection->second];

            this->prev_delta2_[i] *= scale_factor_ * scale_factor_ * prev_h.df(prev_out[i]) * prev_h.df(prev_out[i]);
        }
        return this->prev_->back_propagation_2nd(this->prev_delta2_);
    }

    // remove unused weight to improve cache hits
    void remap() {
        std::map<int, int> swaps;
        int n = 0, i;

        for (i = 0; i < weight2io_.size(); i++)
            swaps[i] = weight2io_[i].empty() ? -1 : n++;

        for (i = 0; i < this->out_size_; i++) {
            wi_connections& wi = out2wi_[i];
            for (size_t j = 0; j < wi.size(); j++)
                wi[j].first = swaps[wi[j].first];
        }

        for (i = 0; i < this->in_size_; i++) {
            wo_connections& wo = in2wo_[i];
            for (size_t j = 0; j < wo.size(); j++)
                wo[j].first = swaps[wo[j].first];
        }

        std::vector<io_connections> weight2io_new(n);
        for (i = 0; i < weight2io_.size(); i++)
            if(swaps[i] >= 0) weight2io_new[swaps[i]] = weight2io_[i];

        weight2io_ = weight2io_new;
    }

protected:
    std::vector<io_connections> weight2io_; // weight_id -> [(in_id, out_id)]
    std::vector<wi_connections> out2wi_; // out_id -> [(weight_id, in_id)]
    std::vector<wo_connections> in2wo_; // in_id -> [(weight_id, out_id)]
    std::vector<std::vector<int> > bias2out_;
    std::vector<int> out2bias_;
    float_t scale_factor_;
};

