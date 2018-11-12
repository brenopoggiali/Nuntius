#include "server/channel.h"

channel::channel(std::string name, int max_clients) : _name(name), _max_clients(max_clients), _client_count(0){}

bool channel::add_client(client_ptr& client){
    if(this->_client_count < this->_max_clients){
        this->_clients[client->_nickname] = client;
        this->_client_count++;
        return true;
    }
    return false;
}

bool channel::remove_client(std::string& nickname){
    auto it = this->_clients->find(nickname);
    if(it != this->_clients.end()){

        this->_clients.erase(it);
        return true;
    }
    return false;
}

void channel::send_msg(std::string& msg, std::string nickname){
    for(auto it = this->_clients.begin(), it != this->_clients.end(), it++){
        if(it->first != nickname){
            it->second.send_msg(msg);
        }
    }
}

int channel::get_num_clients(){
    return this->_client_count;
}