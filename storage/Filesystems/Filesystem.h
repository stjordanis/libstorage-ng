/*
 * Copyright (c) [2014-2015] Novell, Inc.
 * Copyright (c) [2016-2017] SUSE LLC
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
 * with this program; if not, contact Novell, Inc.
 *
 * To contact Novell about this file by physical or electronic mail, you may
 * find current contact information at www.novell.com.
 */


#ifndef STORAGE_FILESYSTEM_H
#define STORAGE_FILESYSTEM_H


#include <vector>

#include "storage/Filesystems/Mountable.h"


namespace storage
{
    class SpaceInfo;


    // abstract class

    class Filesystem : public Mountable
    {
    public:

	static std::vector<Filesystem*> get_all(Devicegraph* devicegraph);
	static std::vector<const Filesystem*> get_all(const Devicegraph* devicegraph);

	FsType get_type() const;

	bool has_space_info() const;

	/**
	 * So far only supported for Nfs.
	 */
	SpaceInfo detect_space_info() const;

	/**
	 * Set the SpaceInfo. Only use for testsuites.
	 */
	void set_space_info(const SpaceInfo& space_info);

	static std::vector<Filesystem*> find_by_mountpoint(const Devicegraph* devicegraph,
							   const std::string& mountpoint);

    public:

	class Impl;

	Impl& get_impl();
	const Impl& get_impl() const;

    protected:

	Filesystem(Impl* impl);

    };


    bool is_filesystem(const Device* device);

    Filesystem* to_filesystem(Device* device);
    const Filesystem* to_filesystem(const Device* device);

}

#endif