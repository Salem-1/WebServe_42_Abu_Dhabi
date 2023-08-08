
#include "Receive.hpp"

Receive::Receive(): state(KEEP_ALIVE)
{

};

Receive::Receive(int read_sock): read_sock(read_sock), state(KEEP_ALIVE)
{
};

Receive &Receive::operator= (const Receive &obj2)
{
    if (this != &obj2)
    {
        this->read_sock = obj2.read_sock;
        this->bytes_read = obj2.bytes_read;
        this->state = obj2.state;
    }
    return (*this);
}
Receive:: Receive(const Receive &obj2)
{
    *this = obj2;
}

Receive::~Receive()
{}

void    Receive::receiveAll()
{
    memset(buffer, 0, READ_BUFFER_SIZE);
    readPacket(buffer);
    parser.setBytereadAndReadsock(bytes_read, read_sock);
    parser.parse(buffer);
    vis_str(buffer, "inside recievea;");
    if (bytes_read == 0)
    {
        std::cout << "byter read = " << bytes_read << "will kill connection\n";
        state = KILL_CONNECTION;
        return ;
    }
    if (parser.read_again)
        state = KEEP_ALIVE;
    return ;
}


void    Receive::readPacket(char *buffer)
{
    bytes_read = recv(read_sock, buffer, READ_BUFFER_SIZE - 1, 0);
    if (strlen(buffer) < 10000)
        std::cout << bytes_read <<" bytes_read buffer is\n"<< buffer  << std::endl;
    else
        std::cout << "recieved a large packet\n";
    if (bytes_read == -1)
    {
        perror("recv Error: ");
        state = KILL_CONNECTION;
        bytes_read= 0;
        return ;
    }
}
// ?? it is already public
packet_map       &Receive::get_request_packet()
{
    return (parser.request);
}