#ifndef CHANNEL_H
#define CHANNEL_H

#include <memory>
#include <map>

#include "server/client.h"

typedef boost::shared_ptr<TCPclient> client_ptr;

class channel{
    private:
        std::string _name;
        int _client_count;
        int _max_clients;
        std::map<std::string, client_ptr> _clients;
    
    public:
        channel::channel(std::string name);
        //pensar sobre isso dps
        //channel::~channel();

        bool add_client(client_ptr& client);
        bool remove_client(std::string nickname);
        int get_num_clients();
        void send_msg(std::string& msg, std::string nickname);

};

#endif