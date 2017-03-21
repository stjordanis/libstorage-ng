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
 * with this program; if not, contact Novell, Inc.
 *
 * To contact Novell about this file by physical or electronic mail, you may
 * find current contact information at www.novell.com.
 */


#ifndef STORAGE_ISO9660_H
#define STORAGE_ISO9660_H


#include "storage/Filesystems/BlkFilesystem.h"


namespace storage
{

    /**
     * Class for ISO9660 filesystem. The library cannot create an ISO9660
     * filesystem on disk.
     */
    class Iso9660 : public BlkFilesystem
    {
    public:

	static Iso9660* create(Devicegraph* devicegraph);
	static Iso9660* load(Devicegraph* devicegraph, const xmlNode* node);

    public:

	class Impl;

	Impl& get_impl();
	const Impl& get_impl() const;

	virtual Iso9660* clone() const override;

    protected:

	Iso9660(Impl* impl);

    };


    bool is_iso9660(const Device* device);

    Iso9660* to_iso9660(Device* device);
    const Iso9660* to_iso9660(const Device* device);

}

#endif