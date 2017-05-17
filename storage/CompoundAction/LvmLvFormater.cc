/*
 * Copyright (c) 2017 SUSE LLC
 *
 * All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, contact SUSE LLC.
 *
 * To contact SUSE LLC about this file by physical or electronic mail, you may
 * find current contact information at www.suse.com.
 */


#include "storage/CompoundAction/LvmLvFormater.h"


namespace storage
{

    LvmLvFormater::LvmLvFormater(const CompoundAction::Impl* compound_action)
    : CompoundActionFormater(compound_action) 
    {
	this->lv = to_lvm_lv(compound_action->get_target_device());
    }


    LvmLvFormater::~LvmLvFormater() {}

    
    Text
    LvmLvFormater::text() const
    {
        Text text = tenser(tense,
                           _("String representation for a LvmLv target (pending)"),
                           _("String representation for a LvmLv target (pending)"));

        return sformat(text);
    }


//
//    Text
//    CompoundAction::Impl::create_lvm_lv_text(const LvmLv* lv, Tense tense) const
//    {
//	auto vg = lv->get_lvm_vg();
//	auto filesystem = lv->get_blk_filesystem();
//
//	Text text = tenser(tense,
//			   _("Create LVM logical volume %1$s (%2$s) on volume group %3$s for %4$s with %5$s"),
//			   _("Creating LVM logical volume %1$s (%2$s) on volume group %3$s for %4$s with %5$s"));
//
//	return sformat(text, 
//		       lv->get_name().c_str(), 
//		       lv->get_size_string().c_str(),
//		       vg->get_vg_name().c_str(),
//		       filesystem->get_mount_point()->get_path().c_str(),
//		       filesystem->get_displayname().c_str());
//    }
//

}

