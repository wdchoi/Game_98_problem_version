/////////////////////////////////////////////////////////////////////////////
// Authored by Jeong-Mo Hong for CSE4060 course at Dongguk University CSE  //
// jeongmo.hong@gmail.com                                                  //
// Do whatever you want license.                                           //
/////////////////////////////////////////////////////////////////////////////

#pragma once

template<class TT>
class CircularQueue
{
public:
    int tail_;
    bool full_;
    Array1D<TT> array_;
    
    void initialize(const int& num_max)
    {
        tail_ = 0;
        full_ = false;

        array_.initialize(num_max);
    }

    void pushBack(const TT& input)
    {
        array_[tail_] = input;

        tail_++;

        if (full_ == false && tail_ >= array_.num_elements_) full_ = true;

        tail_ = tail_ % array_.num_elements_;
    }

    TT& getValue(const int& index)
    {
        return array_.values_[(tail_ + index) % array_.num_elements_];// avilable when queue is full
    }

    TT& getFirst()
    {
        if (full_ == false) return array_.values_[0];
        else return getValue(0);
    }

    TT& getLast()
    {
        return getValue(-1);
    }
};