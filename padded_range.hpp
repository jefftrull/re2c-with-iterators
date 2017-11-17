/*=============================================================================

    Definition of the padded range - a wrapper adding extra dummy values

    Copyright (c) 2017 Jeffrey E Trull. Distributed under the Boost
    Software License, Version 1.0. (See accompanying file
    LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(PADDED_RANGE_HPP_INCLUDED)
#define PADDED_RANGE_HPP_INCLUDED

#include <iterator>
#include <boost/iterator/iterator_facade.hpp>

///////////////////////////////////////////////////////////////////////////////
//
//  padded_range
//
//      A range consisting of two ranges concatenated: a source range, and a
//      repeated value of the same type.  Together we get an iterator over
//      the original range, with padding at the end.
//
//      An alternative is to use Boost.Range join with iterator_range and
//      function_input_iterator, but that produces a forward iterator, even
//      for random access inputs. Our approach here yields a range/iterator of
//      the same category as the underlying source.
//
///////////////////////////////////////////////////////////////////////////////

template<typename SrcIt,                                        // source iterator type
         typename std::iterator_traits<SrcIt>::value_type Pad,  // value to use as padding
         std::size_t Count>                                     // how many to supply
struct padded_range
{

    // BOZO use C++14/17 where possible

    typedef typename std::iterator_traits<SrcIt>::value_type      value_type;
    typedef typename std::iterator_traits<SrcIt>::difference_type difference_type;

    // first, an iterator that can refer to our internals
    struct iterator
    : boost::iterator_facade<
          iterator,
          typename std::iterator_traits<SrcIt>::value_type,
          typename std::iterator_traits<SrcIt>::iterator_category,
          // reference type is *const* due to fake padding
          typename std::iterator_traits<SrcIt>::value_type const&>
    {
        iterator(padded_range const * rng, SrcIt pos, std::size_t pad_idx)
            : rng_(rng), pos_(pos), pad_idx_(pad_idx) {}

    private:

        //
        // iterator_facade requirements
        //

        friend class boost::iterator_core_access;
    
        // for ForwardIterators

        value_type const& dereference() const
        {
            if (pos_ != rng_->src_end_) {
                // inside the source range
                return *pos_;
            } else {
                // inside the padding
                return rng_->pad_;
            }
        }

        bool equal(iterator const& other) const
        {
            return ((pos_ == other.pos_) && (pad_idx_ == other.pad_idx_));
        }

        void increment()
        {
            advance(1);
        }
        
        // for BiDirectionalIterators

        void decrement() {
            advance(-1);
        }

        // for RandomAccessIterators

        // caller ensures n is within range
        void advance(difference_type n) {
            // shortcut for common case: increment within source range
            // std::distance(pos_, rng_->src_end_) will be expensive for forward ranges...
            if ((n == 1) && (pos_ != rng_->src_end_)) {
                ++pos_;
                return;
            }

            // uses std::distance/std::advance, so valid for bidirectional+forward
            if (pos_ != rng_->src_end_) {
                // we are in the source range
                if (n < 0) {
                    std::advance(pos_, n);    // UB if goes past src_beg_
                } else {
                    // moving forward, so we may cross into padding
                    difference_type remaining_src =
                        std::distance(pos_, rng_->src_end_);
                    if (n <= remaining_src) {
                        std::advance(pos_, n);
                    } else {
                        pos_ = rng_->src_end_;
                        pad_idx_ += (n - remaining_src);
                    }
                }
            } else {
                // we are in the padding
                if (n >= 0) {
                    pad_idx_ += n;             // UB if it goes past src_end_
                } else {
                    // do we move back into the source range?
                    if ((std::size_t)(-n) > pad_idx_) {
                        // this will take us out of the padding
                        n += pad_idx_;         // reduce n by remaining padding
                        pad_idx_ = 0;
                        std::advance(pos_, n); // move within source for the rest
                    } else {
                        pad_idx_ += n;
                    }
                }
            }
        }

        difference_type distance_to(iterator const& other) const {
            return std::distance(pos_, other.pos_) +
                (difference_type)(other.pad_idx_) - (difference_type)(pad_idx_);
        }

        padded_range<SrcIt, Pad, Count> const * rng_;  // parent range
        SrcIt            pos_;                         // position within src
        std::size_t      pad_idx_;                     // position within padding

    };

    padded_range(SrcIt beg, SrcIt end) : src_beg_(beg), src_end_(end), pad_(Pad) {}

    iterator begin() const {
        return iterator(this, src_beg_, 0);
    }

    iterator end_input() const {
        return iterator(this, src_end_, 0);
    }

    iterator end() const {
        return iterator(this, src_end_, Count);
    }

private:
    SrcIt            src_beg_;
    SrcIt            src_end_;
    value_type const pad_;
};

///////////////////////////////////////////////////////////////////////////////

#endif // !defined(PADDED_RANGE_HPP_INCLUDED)
