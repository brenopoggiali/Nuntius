#ifndef CHANNEL_H
#define CHANNEL_H

#include <memory>
#include <map>

#include "client.h"

#define MAX_CLIENTS 5

class channel{
    public:
        std::string _name;
        int _max_clients;
        std::map<std::string, TCPclient*> _clients;
    
        channel(std::string name);
        ~channel();

        bool add_client(TCPclient* client);
        bool remove_client(std::string& nickname);
        bool can_recv_client();
        int get_num_clients();
        void send_msg(std::string& msg, std::string nickname);

};

#endif