#include "channel.h"

channel::channel(std::string name) : _name(name), _max_clients(MAX_CLIENTS){
    std::cout << "channel criado: " << name << std::endl;
}

channel::~channel() {
    std::cout << "channel destruido: " << this->_name << std::endl;
}

bool channel::add_client(TCPclient* client){
    if(this->get_num_clients() < this->_max_clients){
        this->_clients.insert(std::make_pair(client->_nickname, client));
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

void channel::send_msg(std::string& msg, std::string client_nickname){
    for(auto it = this->_clients.begin(); it != this->_clients.end(); it++){
        if(it->first != client_nickname){
            std::string formated_msg = client_nickname + ": "+msg;
            it->second->send_msg(formated_msg);
        }
    }
}

bool channel::can_recv_client(){
    return this->get_num_clients() < this->_max_clients;
}

int channel::get_num_clients(){
    return this->_clients.size();
}