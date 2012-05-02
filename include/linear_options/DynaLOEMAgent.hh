#ifndef __DYNA_LOEM_AGENT_H__
#define __DYNA_LOEM_AGENT_H__

#include <linear_options/LOEMAgent.hh>
#include <map>

namespace rl {

/**
 * Implements the agent described in the paper
 * "Linear Options" that interleaves one step of 
 * planing update with one step of intra-option learning
 * for all the options with their attached LOEM model.
 */
class DynaLOEMAgent : public LOEMAgent
{
public:
     DynaLOEMAgent();
    ~DynaLOEMAgent();

    /**
     * @Override
     */
    int first_action(const std::vector<float> &s);

    /**
     * @Override
     */
    int next_action(float r, const std::vector<float> &s);

    /**
     * @Override
     */
    void last_action(float r);

    /**
     * @Override
     */
    void setDebug(bool d);

protected:    
    /**
     * Return the action with the highest return max_o Q(s, O)
     * @param phi The n-dimensional projection of a state
     * @param the LinearOption of maximum value
     */
    LinearOption* getBestOption(const Eigen::VectorXd& phi);

    // We maintain a model of the transition and reward dynamics for every option.
    std::map<LinearOption*, LinearOptionModel*> optionModels;

private:
    // Last action executed
    int lastAction;

    // Last state visited
    Eigen::VectorXd lastPhi;

    // The option that we are currently executing up to termination
    LinearOption* currentOption;

    // Maximum value for next state from s
    struct NextStateValueComparator {
        NextStateValueComparator(DynaLOEMAgent* self, const Eigen::VectorXd& phi) : self(self), phi(phi) {}

        bool operator() (LinearOption*& a, LinearOption*& b) { 
            return a->theta.transpose()*self->optionModels[a]->F*phi < b->theta.transpose()*self->optionModels[b]->F*phi;     
        }

        DynaLOEMAgent* self;
        const Eigen::VectorXd& phi;
    };

    // Choose new option according to main behavior policy 
    struct ValueComparator {
        ValueComparator(const Eigen::VectorXd& phi) : phi(phi) {}
        bool operator() (LinearOption*& a, LinearOption*& b) { 
            return a->theta.transpose()*phi < b->theta.transpose()*phi;     
        }
        const Eigen::VectorXd& phi;
    };
};

}
#endif
