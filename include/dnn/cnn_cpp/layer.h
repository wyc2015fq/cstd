
#pragma once
#include "util.h"
#include "product.h"

// base class of all kind of NN layers
struct layer {
    Activation a_;
    int in_size_;
    int out_size_;
    bool parallelize_;

    struct layer* next_;
    struct layer* prev_;
    vec_t output_[CNN_TASK_SIZE];     // last output of current layer, set by fprop
    vec_t prev_delta_[CNN_TASK_SIZE]; // last delta of previous layer, set by bprop
    vec_t W_;          // weight vector
    vec_t b_;          // bias vector
    vec_t dW_[CNN_TASK_SIZE];
    vec_t db_[CNN_TASK_SIZE];

    vec_t Whessian_; // diagonal terms of hessian matrix
    vec_t bhessian_;
    vec_t prev_delta2_; // d^2E/da^2
    //typedef typename Network::Optimizer Optimizer;
    //typedef typename Network::LossFunction LossFunction;

    layer(int in_dim, int out_dim, int weight_dim, int bias_dim) : parallelize_(true), next_(0), prev_(0) {
        set_size(in_dim, out_dim, weight_dim, bias_dim);
    }

    void connect(struct layer* tail) {
        if (this->out_size() != 0 && tail->in_size() != this->out_size())
            throw nn_error("dimension mismatch");
        next_ = tail;
        tail->prev_ = this;
    }

    void set_parallelize(bool parallelize) {
        parallelize_ = parallelize;
    }

    // cannot call from ctor because of pure virtual function call fan_in_size().
    // so should call this function explicitly after ctor
    void init_weight() {
        const float_t weight_base = 0.5 / sqrt(fan_in_size());

        uniform_rand(W_.begin(), W_.end(), -weight_base, weight_base);
        uniform_rand(b_.begin(), b_.end(), -weight_base, weight_base);               
        std::fill(Whessian_.begin(), Whessian_.end(), 0.0);
        std::fill(bhessian_.begin(), bhessian_.end(), 0.0);
        clear_diff(CNN_TASK_SIZE);
    }

    const vec_t& output(int worker_index) const { return output_[worker_index]; }
    const vec_t& delta(int worker_index) const { return prev_delta_[worker_index]; }
    vec_t& weight() { return W_; }
    vec_t& bias() { return b_; }

    void divide_hessian(int denominator) {
      vec_t::iterator w, b;
        for (w=Whessian_.begin(); w!=Whessian_.end(); ++w) *w /= denominator;
        for (b=bhessian_.begin(); b!=bhessian_.end(); ++b) *b /= denominator;
    }

    virtual int in_size() const { return in_size_; }
    virtual int out_size() const { return out_size_; }
    virtual int param_size() const { return W_.size() + b_.size(); }
    virtual int fan_in_size() const = 0;
    virtual int connection_size() const = 0;

    virtual void save(std::ostream& os) const {
        for (int w=0; w< W_.size(); ++w) os << W_[w] << " ";
        for (int b=0; b< b_.size(); ++b) os << b_[b] << " ";
    }

    virtual void load(std::istream& is) {
        for (int w=0; w< W_.size(); ++w) is >> W_[w];
        for (int b=0; b< b_.size(); ++b) is >> b_[b];
    }

    virtual const vec_t& forward_propagation(const vec_t& in, int worker_index) = 0;
    virtual const vec_t& back_propagation(const vec_t& current_delta, int worker_index) = 0;
    virtual const vec_t& back_propagation_2nd(const vec_t& current_delta2) = 0;

    layer* next() { return next_; }
    layer* prev() { return prev_; }

    void update_weight(Optimizer *o, int worker_size, int batch_size) {
      if (W_.empty()) {
        return;
      }

        merge(worker_size, batch_size);

        int i;
        for (i = 0; i < W_.size(); i++) {
          o->update(o, dW_[0][i], Whessian_[i], &W_[i]);
        }

        int dim_b = b_.size();
        for (i = 0; i < dim_b; i++)
            o->update(o, db_[0][i], bhessian_[i], &b_[i]);

        clear_diff(worker_size);
    }

    vec_t& weight_diff(int index) { return dW_[index]; }
    vec_t& bias_diff(int index) { return db_[index]; }

	bool has_same_weights(const layer& rhs, float_t eps) const {
        if (W_.size() != rhs.W_.size() || b_.size() != rhs.b_.size())
            return false;
int i;
        for (i = 0; i < W_.size(); i++)
          if (fabs(W_[i] - rhs.W_[i]) > eps) return false;
        for (i = 0; i < b_.size(); i++)
          if (fabs(b_[i] - rhs.b_[i]) > eps) return false;

        return true;
	}

    void merge(int worker_size, int batch_size) {
      int i;
        for (i = 1; i < worker_size; i++)
            vectorize::reduce<float_t>(&dW_[i][0], dW_[i].size(), &dW_[0][0]);
        for (i = 1; i < worker_size; i++) 
            vectorize::reduce<float_t>(&db_[i][0], db_[i].size(), &db_[0][0]);
        //for (int i = 1; i < worker_size; i++) {
        //    std::transform(dW_[0].begin(), dW_[0].end(), dW_[i].begin(), dW_[0].begin(), std::plus<float_t>());
        //    std::transform(db_[0].begin(), db_[0].end(), db_[i].begin(), db_[0].begin(), std::plus<float_t>());
        //}

        float_t *p;
        for (p = dW_[0].begin(); p!=dW_[0].end(); p++) {*p /= batch_size; }
        for (p = db_[0].begin(); p!=db_[0].end(); p++) {*p /= batch_size; }
    }

    void clear_diff(int worker_size) {
        for (int i = 0; i < worker_size; i++) {
            std::fill(dW_[i].begin(), dW_[i].end(), 0.0);
            std::fill(db_[i].begin(), db_[i].end(), 0.0);
        }
    }

    void set_size(int in_dim, int out_dim, int weight_dim, int bias_dim) {
        in_size_ = in_dim;
        out_size_ = out_dim;

        for (int o=0; o<CNN_TASK_SIZE; ++o)
            output_[o].resize(out_dim);
        for (int p=0; p<CNN_TASK_SIZE; ++p)
            prev_delta_[p].resize(in_dim);
        W_.resize(weight_dim);
        b_.resize(bias_dim);     
        Whessian_.resize(weight_dim);
        bhessian_.resize(bias_dim);
        prev_delta2_.resize(in_dim);

        for (int dw=0; dw<CNN_TASK_SIZE; ++dw)
            dW_[dw].resize(weight_dim);

        for (int db=0; db < CNN_TASK_SIZE; ++db)
            db_[db].resize(bias_dim);
    }
    Activation& activation_function() { return a_; }
};

class input_layer : public layer {
public:
    typedef layer Base;
    //typedef typename Base::Optimizer Optimizer;

    input_layer() : layer(0, 0, 0, 0) {}

    int in_size() const { return this->next_ ? this->next_->in_size(): 0; }

    const vec_t& forward_propagation(const vec_t& in, int index) {
        this->output_[index] = in;
        return this->next_ ? this->next_->forward_propagation(in, index) : this->output_[index];
    }

    const vec_t& back_propagation(const vec_t& current_delta, int index) {
        return current_delta;
    }

    const vec_t& back_propagation_2nd(const vec_t& current_delta2) {
        return current_delta2;
    }

    int connection_size() const {
        return this->in_size_;
    }

    int fan_in_size() const {
        return 1;
    }
};


class layers {
public:
    layers() {
        add(&first_);
    }

    void add(layer* new_tail) {
        if (tail())  tail()->connect(new_tail);
        layers_.push_back(new_tail);
    }

    bool empty() const { return layers_.size() == 0; }

    layer* head() const { return empty() ? 0 : layers_[0]; }

    layer* tail() const { return empty() ? 0 : layers_[layers_.size() - 1]; }

    void reset() {
      std::vector<layer*>::const_iterator pl;
      for (pl=layers_.begin(); pl!=layers_.end(); ++pl)
            (*pl)->init_weight();
    }

    void divide_hessian(int denominator) {
      std::vector<layer*>::const_iterator pl;
        for (pl = layers_.begin(); pl!=layers_.end(); ++pl)
            (*pl)->divide_hessian(denominator);
    }

    void update_weights(Optimizer *o, int worker_size, int batch_size) {
      std::vector<layer*>::const_iterator pl;
        for (pl = layers_.begin(); pl!=layers_.end(); ++pl)
            (*pl)->update_weight(o, worker_size, batch_size);
    }
    
    void set_parallelize(bool parallelize) {
      std::vector<layer*>::const_iterator pl;
        for (pl = layers_.begin(); pl!=layers_.end(); ++pl)
            (*pl)->set_parallelize(parallelize);
    }

private:
    std::vector<layer*> layers_;
    input_layer first_;
};

template <typename Char, typename CharTraits>
std::basic_ostream<Char, CharTraits>& operator << (std::basic_ostream<Char, CharTraits>& os, const layer& v) {
    v.save(os);
    return os;
}

template <typename Char, typename CharTraits>
std::basic_istream<Char, CharTraits>& operator >> (std::basic_istream<Char, CharTraits>& os, layer& v) {
    v.load(os);
    return os;
}
