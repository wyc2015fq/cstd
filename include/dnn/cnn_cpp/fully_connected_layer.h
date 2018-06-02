
#pragma once
#include "layer.h"
#include "product.h"

// normal 
template<typename N,typename Optimizer,typename LossFunction, typename Activation>
class fully_connected_layer : public layer<N, Optimizer, LossFunction, Activation> {
public:
    typedef layer<N, Optimizer, LossFunction, Activation> Base;
    //typedef typename Base::Optimizer Optimizer;

    fully_connected_layer(int in_dim, int out_dim) : layer<N, Optimizer, LossFunction, Activation>(in_dim, out_dim, in_dim * out_dim, out_dim) {}

    int connection_size() const {
        return this->in_size_ * this->out_size_ + this->out_size_;
    }

    int fan_in_size() const {
        return this->in_size_;
    }

    const vec_t& forward_propagation(const vec_t& in, int index) {

        for (int r = 0; r < this->out_size_; r++) {
            float_t z = 0.0;
            for (int c = 0; c < this->in_size_; c++) 
                z += this->W_[c*this->out_size_+r] * in[c];

            z += this->b_[r];
            this->output_[index][r] = this->a_.f(z);
        }

        return this->next_ ? this->next_->forward_propagation(this->output_[index], index) : this->output_[index];
    }

    const vec_t& back_propagation(const vec_t& current_delta, int index) {
        const vec_t& prev_out = this->prev_->output(index);
        const Activation& prev_h = this->prev_->activation_function();
        vec_t& prev_delta = this->prev_delta_[index];
        vec_t& dW = this->dW_[index];
        vec_t& db = this->db_[index];
        int i, c;

        for (c = 0; c < this->in_size_; c++) {
            //prev_delta[c] = 0.0;
            //for (int r = 0; r < this->out_size_; r++)
            //    prev_delta[c] += current_delta[r] * this->W_[c*this->out_size_+r];

            prev_delta[c] = vectorize::dot(&current_delta[0], &this->W_[c*this->out_size_], this->out_size_);
            prev_delta[c] *= prev_h.df(prev_out[c]);
        }

            /*for (int c = 0; c < this->in_size_; c++) 
                for (int i = r.begin(); i < r.end(); i++) 
                    dW[c*this->out_size_+i] += current_delta[i] * prev_out[c];*/

            for (c = 0; c < this->in_size_; c++) {
                vectorize::muladd(&current_delta[0], prev_out[c], this->out_size_, &dW[c*this->out_size_]);
            }

            for (i = 0; i < this->out_size_; i++) 
                db[i] += current_delta[i];

        return this->prev_->back_propagation(this->prev_delta_[index], index);
    }


    const vec_t& back_propagation_2nd(const vec_t& current_delta2) {
        const vec_t& prev_out = this->prev_->output(0);
        const Activation& prev_h = this->prev_->activation_function();

        int r, c;

        for (c = 0; c < this->in_size_; c++) 
            for (int r = 0; r < this->out_size_; r++)
                this->Whessian_[c*this->out_size_+r] += current_delta2[r] * prev_out[c] * prev_out[c];

        for (r = 0; r < this->out_size_; r++)
            this->bhessian_[r] += current_delta2[r];

        for (c = 0; c < this->in_size_; c++) { 
            this->prev_delta2_[c] = 0.0;

            for (r = 0; r < this->out_size_; r++) 
                this->prev_delta2_[c] += current_delta2[r] * this->W_[c*this->out_size_+r] * this->W_[c*this->out_size_+r];

            this->prev_delta2_[c] *= prev_h.df(prev_out[c]) * prev_h.df(prev_out[c]);
        }

        return this->prev_->back_propagation_2nd(this->prev_delta2_);
    }
};
