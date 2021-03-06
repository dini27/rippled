//------------------------------------------------------------------------------
/*
    This file is part of rippled: https://github.com/ripple/rippled
    Copyright (c) 2012, 2013 Ripple Labs Inc.

    Permission to use, copy, modify, and/or distribute this software for any
    purpose  with  or without fee is hereby granted, provided that the above
    copyright notice and this permission notice appear in all copies.

    THE  SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
    WITH  REGARD  TO  THIS  SOFTWARE  INCLUDING  ALL  IMPLIED  WARRANTIES  OF
    MERCHANTABILITY  AND  FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
    ANY  SPECIAL ,  DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
    WHATSOEVER  RESULTING  FROM  LOSS  OF USE, DATA OR PROFITS, WHETHER IN AN
    ACTION  OF  CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/
//==============================================================================


#ifndef RIPPLE_APP_RPCHTTPSERVER_H_INCLUDED
#define RIPPLE_APP_RPCHTTPSERVER_H_INCLUDED

class RPCHTTPServer : public Stoppable
{
protected:
    RPCHTTPServer (Stoppable& parent);

public:
    static RPCHTTPServer* New (Stoppable& parent,
        Journal journal, JobQueue& jobQueue, NetworkOPs& networkOPs);

    virtual ~RPCHTTPServer () { }

    /** Opens listening ports based on the Config settings. */
    virtual void setup (Journal journal) = 0;
};

#endif
