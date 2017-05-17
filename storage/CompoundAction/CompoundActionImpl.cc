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


#include <boost/algorithm/string/join.hpp>

#include "storage/CompoundAction/CompoundActionImpl.h"
#include "storage/CompoundAction/CompoundActionGenerator.h"
#include "storage/CompoundAction/PartitionFormater.h"
#include "storage/CompoundAction/LvmLvFormater.h"
#include "storage/CompoundAction/LvmVgFormater.h"
#include "storage/CompoundAction/BtrfsSubvolumeFormater.h"
#include "storage/CompoundAction/BtrfsFormater.h"
#include "storage/CompoundAction/NfsFormater.h"
#include "storage/ActiongraphImpl.h"
#include "storage/Devices/PartitionTable.h"
#include "storage/Devices/Partitionable.h"
#include "storage/Devices/Encryption.h"
#include "storage/Devices/LvmPv.h"
#include "storage/Filesystems/BlkFilesystem.h"
#include "storage/Filesystems/MountPoint.h"


namespace storage
{

    CompoundAction::Impl::Impl(const Actiongraph* actiongraph)
    : actiongraph(actiongraph), target_device(nullptr), commit_actions(0) 
    {}

    
    CompoundAction::Impl::Impl(const Actiongraph* actiongraph, const Action::Base* action)
    : actiongraph(actiongraph), target_device(nullptr), commit_actions(0) 
    {
	set_target_device(get_target_device(actiongraph, action));
	add_commit_action(action);
    }

    
    CompoundAction::Impl::~Impl() {}

    
    const Actiongraph*
    CompoundAction::Impl::get_actiongraph() const
    {
	return actiongraph;
    }

    
    void CompoundAction::Impl::set_target_device(const Device* device)
    {
	this->target_device = device;
    }


    const Device* CompoundAction::Impl::get_target_device() const
    {
	return target_device;
    }

    void CompoundAction::Impl::set_commit_actions(vector<const Action::Base*> actions)
    {
	this->commit_actions = actions;
    }


    vector<const Action::Base*> CompoundAction::Impl::get_commit_actions() const
    {
	return commit_actions;
    }


    void CompoundAction::Impl::add_commit_action(const Action::Base* action)
    {
	commit_actions.push_back(action);
    }


    vector<string>
    CompoundAction::Impl::get_commit_actions_as_strings() const
    {
	const CommitData commit_data(actiongraph->get_impl(), Tense::SIMPLE_PRESENT);

	vector<string> ret;
	for (const Action::Base* action : commit_actions)
	    ret.push_back(action->text(commit_data).translated);

	return ret;
    }


    //TODO Generate text depending on the set of commit actions.
    string
    CompoundAction::Impl::string_representation() const
    {
	if (is_partition(target_device))
	    return PartitionFormater(this).string_representation();

	else if (is_lvm_lv(target_device))
	    return LvmLvFormater(this).string_representation();

	else if (is_lvm_vg(target_device))
	    return LvmVgFormater(this).string_representation();

	else if (is_btrfs(target_device))
	    return BtrfsFormater(this).string_representation();

	else if (is_btrfs_subvolume(target_device))
	    return BtrfsSubvolumeFormater(this).string_representation();

	else if (is_nfs(target_device))
	    return NfsFormater(this).string_representation();

	else
	    return boost::algorithm::join(get_commit_actions_as_strings(), " and ");	
    }


    // static methods
    
    vector<CompoundAction*> 
    CompoundAction::Impl::generate(const Actiongraph* actiongraph)
    {
	return CompoundActionGenerator(actiongraph).generate();	 
    
    }


    const Device*
    CompoundAction::Impl::get_target_device(const Actiongraph* actiongraph, const Action::Base* action)
    {
	return get_target_device(device(actiongraph, action));
    }


    const Device*
    CompoundAction::Impl::get_target_device(const Device* device)
    {
	if (is_partition_table(device))
	    return get_target_device(to_partition_table(device));

	if (is_encryption(device))
	    return get_target_device(to_encryption(device));

	if (is_lvm_pv(device))
	    return get_target_device(to_lvm_pv(device));

	if (is_blk_filesystem(device))
	    return get_target_device(to_blk_filesystem(device));

	if (is_mount_point(device))
	    return get_target_device(to_mount_point(device));

	return device;
    }
    

    const Device*
    CompoundAction::Impl::get_target_device(const PartitionTable* partition_table)
    {
	return partition_table->get_partitionable();
    }


    const Device*
    CompoundAction::Impl::get_target_device(const Encryption* encryption)
    {
	return encryption->get_blk_device();
    }


    const Device*
    CompoundAction::Impl::get_target_device(const LvmPv* pv)
    {
	return get_target_device(pv->get_blk_device());
    }


    const Device*
    CompoundAction::Impl::get_target_device(const BlkFilesystem* blk_filesystem)
    {
	auto blk_devices = blk_filesystem->get_blk_devices(); 

	if (blk_devices.size() > 1)
	    // BtrFS with several devices
	    return blk_filesystem;
	    
	return get_target_device(blk_devices.front());
    }


    const Device*
    CompoundAction::Impl::get_target_device(const MountPoint* mount_point)
    {
	return get_target_device(mount_point->get_mountable());
    }


    const Device*
    CompoundAction::Impl::device(const Actiongraph* actiongraph, const Action::Base* action)
    {
	if (is_create(action))
	    return device(actiongraph, dynamic_cast<const Action::Create*>(action));

	if (is_modify(action))
	    return device(actiongraph, dynamic_cast<const Action::Modify*>(action)); 

	if (is_delete(action))
	    return device(actiongraph, dynamic_cast<const Action::Delete*>(action));
    }


    const Device*
    CompoundAction::Impl::device(const Actiongraph* actiongraph, const Action::Create* action)
    {
	return action->get_device(actiongraph->get_impl());
    }


    const Device*
    CompoundAction::Impl::device(const Actiongraph* actiongraph, const Action::Modify* action)
    {
	try
	{
	    return action->get_device(actiongraph->get_impl(), RHS);
	
	}
	catch(const DeviceNotFound& e)
	{
	
	    return action->get_device(actiongraph->get_impl(), LHS);
	}
    }

    
    const Device*
    CompoundAction::Impl::device(const Actiongraph* actiongraph, const Action::Delete* action)
    {
	return action->get_device(actiongraph->get_impl());
    }

}

