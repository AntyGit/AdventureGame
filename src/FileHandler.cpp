#include "FileHandler.h"
#include <iostream>
#include <sstream>
#include <utility>

using namespace std;

bool FileHandler::savePlayer(const string& filename,const Player& player)
{
 stringstream ss;
 string targetfile = filename + player_extension;
 //ios_base::openmode mode = ((m_oldFile && m_oldFilename == filename) ? ios::trunc : ios::app);
 ios_base::openmode mode = ios::trunc;
 m_file.open(targetfile,ios::out | mode);

if(m_file.is_open())
{
     //Rad 1
     ss<<player.getPosition().x<<" "<<player.getPosition().y<<" "<<player.getHealth()<<"\n";

     //Rad 2 fram till items
     auto statuses = player.getStatuses();
     for(auto s : statuses)
        ss<<serializeStatus(s);
     ss<<"\n";

     //items
     auto items = player.getInventory();
        for(auto i:items)
        {
            ss<<serializeItem(i);
            ss<<"\n";
        }

     //cout<<ss.str(); //F�r debug
     m_file<<ss.str();

     m_file.close();
     return true;
 }

 return false;
}

void FileHandler::loadPlayer(const string& filename,Player& player)
{
    stringstream ss;
    string targetfile = filename + player_extension;
    string line;
    int health;
    unsigned stat;
    unsigned duration;
    Position pos;
    //Player player;

    m_file.open(targetfile,ios::in );

    if(m_file.is_open())
    {
        //rad 1
        getline(m_file,line);
        ss<<line;
        ss>>pos.x>>pos.y>>health;
        player.setHealth(health);
        player.setPosition(pos);
        ss.clear();

        //rad 2
        getline(m_file,line);
        ss<<line;
        while(ss>>stat>>duration)
        {
            Status s(static_cast<eStatus>(stat),duration);
            player.addStatus(s);
        }
        ss.clear();

        //items
        while(getline(m_file,line))
        {
            //getline(m_file,line);
            ss<<line<<"\n";
            getline(m_file,line);
            ss<<line<<"\n";
            player.addItem(deserializeItem(ss));
            ss.clear();
        }

        m_file.close();
    }

 //return player;
}

bool FileHandler::saveWorld(const string& filename,const World& world)
{
 //ios_base::openmode mode = ((m_oldFile && m_oldFilename == filename) ? ios::trunc : ios::app);
 ios_base::openmode mode = ios::trunc;
 string targetfile = filename + world_extension;
 m_file.open(targetfile,ios::out | mode);

 stringstream ss;
 if(m_file.is_open())
 {
     vector<Position> positions = world.getRoomPositions();

     for(int i = 0; i<positions.size(); ++i)
     {
         Room* p_room = world.getRoom(positions.at(i));
         ss << serializeRoom(positions.at(i),p_room)<<"\n";

     }
     m_file<<ss.str();
     //cout<<ss.str()<<"\n"; //debug
     m_file.close();
     return true;
 }
 return false;
}

void FileHandler::loadWorld(const string& filename, World& world)
{
    Position pos;
    //stringstream ss;
    string targetfile = filename + world_extension;
    string room_str;
    m_file.open(targetfile,ios::in );

    if(m_file.is_open())
    {
        while(m_file)
        {
         m_file>>pos.x>>pos.y>>ws;
         getline(m_file,room_str,';');
         stringstream ss(room_str);
         world.setRoom(pos,deserializeRoom(ss));
        }

        m_file.close();

    }

    else
    {
        world.setRoom(Position(0,0),new Room("Empty Room","This room is a reflection of the emptiness in your heart"));
    }

}


Item FileHandler::deserializeItem(stringstream& ss)
{

    string name;

    getline(ss,name);
    Item item(name);
    unsigned property;

    while(ss>>property)
    {
        //ss>>property;
        item.addProperty(static_cast<eProperty>(property));
    }
    ss.clear();

    return item;

}

/*Status FileHandler::deserializeStatus(stringstream& ss)
{
    unsigned status;
    unsigned short duration;

    ss>>status>>duration;
    Status s(static_cast<eStatus>(status),duration);
    return s;
}*/

std::string FileHandler::serializeStatus(const Status& status)
{
    stringstream ss;
    ss<<static_cast<unsigned>(status.m_status)<<" "<<status.m_duration<<" ";
    return ss.str();
}

Room* FileHandler::deserializeRoom(std::stringstream& ss)
{
    stringstream item_ss;
    string name,description,line;
    getline(ss,name);
    getline(ss,description);
    Room* room = new Room(name,description);
    while(getline(ss,line))
    {
        item_ss<<line<<"\n";
        getline(ss,line);
        item_ss<<line<<"\n";
        room->addItem(deserializeItem(item_ss));
    }

    return room;
}

string FileHandler::serializeItem(const Item& item)
{
 stringstream ss;
 ss<<item.getName()<<"\n";
 vector<eProperty> properties = item.getProperties();
    for(auto p : properties)
        ss<<static_cast<unsigned>(p)<<" ";
    return ss.str();
}

string FileHandler::serializeRoom(Position& pos,Room const* const room)
{
    stringstream ss;
    //position
    ss<<pos.x<<" "<<pos.y<<"\n";

    //namn
    ss<<room->getName()<<"\n";

    //beskrivning
    ss<<room->getDescription()<<"\n";

    //items
    vector<Item> items = room->getItems();
    for(auto i : items)
    {
        ss<<serializeItem(i)<<"\n";
    }
    //avsluta rum
    ss<<";";

    return ss.str();
}
