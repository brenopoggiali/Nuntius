#include "channel.h"

channel::channel(std::string name) : _name(name), _max_clients(5){}

bool channel::add_client(TCPclient& client){
    if(this->get_num_clients() < this->_max_clients){
        this->_clients.insert(std::make_pair(client._nickname, client));
        return true;
    }
    return false;
}

bool channel::remove_client(std::string& nickname){
    auto it = this->_clients.find(nickname);
    if(it != this->_clients.end()){

        this->_clients.erase(it);
        return true;
    }
    return false;
}

void channel::send_msg(std::string& msg, std::string nickname){
    for(auto it = this->_clients.begin(); it != this->_clients.end(); it++){
        if(it->first != nickname){
            it->second.send_msg(msg);
        }
    }
}

bool channel::can_recv_client(){
    return this->get_num_clients() < this->_max_clients;
}

int channel::get_num_clients(){
    return this->_clients.size();
}