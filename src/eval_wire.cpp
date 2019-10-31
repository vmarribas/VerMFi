
#include "VerifTool.h"


void Wire::eval_wire(bool val, gates& eventList){

  if ((!evaluated_) || (val != value_)){
    value_ = val;
    evaluated_ = 1;

    //Triger next event
    if (this->pinTo_ != NULL)
    {
      //Check if the gate is already stored. If it is do not save it again
      if (std::find(eventList.begin(), eventList.end(), this->pinTo_->get_gate()) == eventList.end())
      {
        // std::cout << "Saving gate " << this->pinTo->gate->gateName_ << " to eventList" << std::endl;
        eventList.push_back(this->pinTo_->get_gate());
      }
    }
    // If it is a fanOut trigger rest of the gates as well
    if (this->fanOut_)
    {
      for (size_t i = 0; i < fanOutPins_.size(); ++i)
      {
        //Check if the gate is already stored. If it is do not save it again
        if (std::find(eventList.begin(), eventList.end(), this->fanOutPins_[i]->get_gate()) == eventList.end())
        {
          // std::cout << "Saving gate " << this->fanOutPins[i]->gate->gateName_ << " to eventList" << std::endl;
          eventList.push_back(this->fanOutPins_[i]->get_gate());
        }
      }
    }
  }
}