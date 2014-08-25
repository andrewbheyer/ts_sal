#ifndef OMG_DDS_CORE_DETAIL_STATUS_CONDITION_HPP_
#define OMG_DDS_CORE_DETAIL_STATUS_CONDITION_HPP_

/* Copyright 2010, Object Management Group, Inc.
 * Copyright 2010, PrismTech, Corp.
 * Copyright 2010, Real-Time Innovations, Inc.
 * All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <dds/core/detail/corefwd.hpp>
#include <dds/core/status/State.hpp>
#include <idds/core/ConditionImpl.hpp>


namespace dds { namespace core { namespace cond {namespace detail {

	template <typename ENTITY>
    class StatusCondition : public idds::core::ConditionImpl {
    public:

    	StatusCondition(const ENTITY& the_parent) : parent_(the_parent) { }

        void enabled_statuses(const dds::core::status::StatusMask& status) {
            mask_ = status;
        }

        const dds::core::status::StatusMask enabled_statuses() const {
            return mask_;
        }

        ENTITY parent() const {
			return parent_;
		}
    private:
		ENTITY parent_;
        dds::core::status::StatusMask mask_;
    };

} } } }
#endif  /* OMG_DDS_CORE_DETAIL_STATUS_CONDITION_HPP_ */


