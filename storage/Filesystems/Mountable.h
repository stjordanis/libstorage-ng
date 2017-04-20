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


#ifndef STORAGE_MOUNTABLE_H
#define STORAGE_MOUNTABLE_H


#include <vector>

#include "storage/Devices/Device.h"
#include "storage/Utils/Swig.h"


namespace storage
{

    class MountPoint;


    enum class FsType {
	UNKNOWN, REISERFS, EXT2, EXT3, EXT4, BTRFS, VFAT, XFS, JFS, HFS, NTFS,
	SWAP, HFSPLUS, NFS, NFS4, TMPFS, ISO9660, UDF, NILFS2
    };


    /**
     * The key by which the mount program identifies a mountable
     */
    enum class MountByType {
	DEVICE, UUID, LABEL, ID, PATH
    };


    std::string get_mount_by_name(MountByType mount_by_type);


    // abstract class

    class Mountable : public Device
    {
    public:

	static std::vector<Mountable*> get_all(Devicegraph* devicegraph);
	static std::vector<const Mountable*> get_all(const Devicegraph* devicegraph);

	MountPoint* create_mount_point(const std::string& path);

	bool has_mount_point() const;

	MountPoint* get_mount_point();
	const MountPoint* get_mount_point() const;

	std::vector<std::string> get_mountpoints() const ST_DEPRECATED;
	void set_mountpoints(const std::vector<std::string>& mountpoints) ST_DEPRECATED;
	void add_mountpoint(const std::string& mountpoint) ST_DEPRECATED;

	MountByType get_mount_by() const ST_DEPRECATED;
	void set_mount_by(MountByType mount_by) ST_DEPRECATED;

	std::vector<std::string> get_mount_opts() const ST_DEPRECATED;
	void set_mount_opts(const std::vector<std::string>& mount_opts) ST_DEPRECATED;

	std::vector<std::string> get_fstab_options() const ST_DEPRECATED;
	void set_fstab_options(const std::vector<std::string>& mount_opts) ST_DEPRECATED;

	static std::vector<Mountable*> find_by_mountpoint(const Devicegraph* devicegraph,
							  const std::string& mountpoint) ST_DEPRECATED;

    public:

	class Impl;

	Impl& get_impl();
	const Impl& get_impl() const;

    protected:

	Mountable(Impl* impl);

    };


    bool is_mountable(const Device* device);

    Mountable* to_mountable(Device* device);
    const Mountable* to_mountable(const Device* device);

}

#endif
