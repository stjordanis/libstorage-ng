/*
 * Copyright (c) [2014-2015] Novell, Inc.
 * Copyright (c) 2016 SUSE LLC
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


#ifndef STORAGE_USER_H
#define STORAGE_USER_H


#include "storage/Holders/Holder.h"


namespace storage
{

    class User : public Holder
    {
    public:

	static User* create(Devicegraph* devicegraph, const Device* source, const Device* target);
	static User* load(Devicegraph* devicegraph, const xmlNode* node);

	virtual User* clone() const override;

    public:

	class Impl;

	Impl& get_impl();
	const Impl& get_impl() const;

    protected:

	User(Impl* impl);

    };


    /**
     * Checks whether holder points to an User.
     *
     * @throw NullPointerException
     */
    bool is_user(const Holder* holder);

    /**
     * Converts pointer to Holder to pointer to User.
     *
     * @return Pointer to User.
     * @throw HolderHasWrongType, NullPointerException
     */
    User* to_user(Holder* holder);

    /**
     * @copydoc to_user(Holder*)
     */
    const User* to_user(const Holder* holder);

}

#endif
