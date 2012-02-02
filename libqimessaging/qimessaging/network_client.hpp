/*
** network_client.hpp
** Login : <ctaf42@donnetout>
** Started on  Thu Feb  2 11:59:48 2012 Cedric GESTES
** $Id$
**
** Author(s):
**  - Cedric GESTES <gestes@aldebaran-robotics.com>
**
** Copyright (C) 2012 Cedric GESTES
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

#ifndef   	NETWORK_CLIENT_HPP_
# define   	NETWORK_CLIENT_HPP_

namespace qi {

  class MachineInfo {
  public:
    std::string uuid;
  };

  class NetworkClient {
  public:
    NetworkClient();
    virtual ~NetworkClient();

    void connect(const std::string masterAddress);
    bool disconnect();

    bool waitForConnected(int msecs = 30000);
    bool waitForDisconnected(int msecs = 30000);

    void registerMachine(const qi::MachineInfo& m);
    void unregisterMachine(const qi::MachineInfo& m);

    bool isInitialized() const;

  protected:
    std::string _masterAddress;

    bool _isInitialized;

    /// <summary> The qi Context</summary>
    qi::Context*                _qiContextPtr;

    /// <summary> The transport client used to talk with the master </summary>
    qi::TransportSocket _transportClient;
  };
}


#endif	    /* !NETWORK_CLIENT_PP_ */
