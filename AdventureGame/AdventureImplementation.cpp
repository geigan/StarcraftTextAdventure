#include <iostream>
#include "AdventureClasses.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

void keyAction::finishGoal(node* location, Map* getMap){
    getMap->completeGoal(goalNum,this);
}

void pickupAction::pickup(node* removeFrom,player* self){
    //std::cout << "testing pickup";
    //std::cout << pickedUp->getName();
    //std::cout << "1";
    removeFrom->removeAction(this->getName());
    //std::cout << "2";
    removeFrom->removeItem(pickedUp->getName());
    //std::cout << "3";
    self->getInventory()->addItem(pickedUp);
    std::cout << this->getDesc() << getDesc();
}

void useAction::use(player* self, Map* getMap){
    if(this->useLocation->getName() == "Cryo-Station"){
        getMap->completeGoal(goalNum,this);
    }else if(self->getLocation()->getDesc() == useLocation->getDesc()){
        getMap->completeGoal(goalNum,this);
    }else{
        std::cout << "You cannot use that item here." << std::endl;
    }
}

void inventory::getInventory(std::ostream& out) const{
    out << "Bag©(v1.02.97), User Storage: ";
    if(int(itemsInventory.size())>0){
        out << itemsInventory[0]->getName();
        for(int i = 1;i<int(itemsInventory.size());i++){
            out << ", " << itemsInventory[i]->getName();
        }
    }else{
        out << "[No items in storage currently]";
    }
    out << std::endl;
}

item* inventory::getItem(std::string itemName){
    for(int i = 0;i<int(itemsInventory.size());i++){
        if(itemsInventory[i]->getName() == itemName) {
            return itemsInventory[i];
        }
    }
    return 0;
};

void inventory::addItem(item* itemAdded){
    itemsInventory.push_back(itemAdded);
};

void inventory::removeItem(std::string itemName){
    for(int i = 0;i<int(itemsInventory.size());i++){
        if(itemsInventory[i]->getName() == itemName){
            itemsInventory.erase(itemsInventory.begin()+i);
        }
    }
};

void node::getActions(std::ostream& out) const{
    out << "Actions: ";
    out << nodeActions[0]->getName();
    for(int i = 1;i<int(nodeActions.size());i++){
        out << ", " << nodeActions[i]->getName();
    }
    out << std::endl;
}

void node::addAction(action* input){
    nodeActions.push_back(input);
}

void node::removeAction(std::string actionName){
    for(int i = 0;i<int(nodeActions.size());i++){
        if(nodeActions[i]->getID() == actionName){
            nodeActions.erase(nodeActions.begin()+i);
        }
    }
};

void node::addItem(item* input){
    itemPickups.push_back(input);
}

void node::removeItem(std::string itemName){
    for(int i = 0;i<int(itemPickups.size());i++){
        if(itemPickups[i]->getName() == itemName){
            itemPickups.erase(itemPickups.begin()+i);
        }
    }
};

node* Map::getNode(node** nodeList,std::string n){
    //std::cout << "getting node";
    for(int i =0;i<7;i++){
        //std::cout << " checking node: " << nodeList[i]->getName();
        if(nodeList[i]->getName()==n){
            return nodeList[i];
        }
    }
    //std::cout << "Error constructing Map nodes." << std::endl;
    return nodeList[0];
}

item* Map::getItem(std::vector<item*>& itemL,std::string n){
    //std::cout << "getting item";
    for(int i =0;i<int(itemL.size());i++){
        //std::cout << " checking item: " << itemList[i]->getName() << "|";
        if(itemList[i]->getName()==n){
            return itemList[i];
        }
    }
    return 0;
}

action* Map::getAction(std::vector<action*>& actionL,std::string n){
    //std::cout << "getting action: " << n;
    //std::cout << actionL.size();
    for(int i =0;i<int(actionL.size());i++){
        //std::cout << "blah";
        //std::cout << " checking action: " << actionL[i]->getID() << "|";
        if(actionL[i]->getID()==n){
            //std::cout << std::endl;
            return actionL[i];
        }
    }
    //std::cout << "Error getting action." << std::endl;
    return errorHandler;
}

action* inventory::checkInventory(std::string n){
    //std::cout << "getting action: " << n;
    if(int(itemsInventory.size())>0){
        for(int i =0;i<int(itemsInventory.size());i++){
            //std::cout << " checking action: " << itemsInventory[i]->getUse()->getName() << "|";
            if(itemsInventory[i]->getUse()->getName()==n){
                //std::cout << std::endl;
                return itemsInventory[i]->getUse();
            }
        }
    }
    //std::cout << "Error getting action." << std::endl;
    return errorHandler;
}

Map::Map(){
    //std::cout << "Loading map..." << std::endl << std::endl;
    //first initialize node list and goal list
    mapNodes = new node*[7];
    goals = new bool[10];
    errorHandler = new action("generic action","err","err","You cannot perform that action here.");
    for(int i = 0; i<10;i++){
        goals[i] = false;
    }
    std::ifstream input1("Nodes.txt");
    int i=0;
    int m=0;
    std::string n;
    std::string d;
    for(std::string line;getline(input1,line); ){
        if((i+5)% 5== 0){
            n = line;
            //std::cout << line << std::endl;
        } else if((i+4)% 5== 0){
            d = line;
            //std::cout << line << std::endl;
        } else if((i+3)% 5== 0){
            //std::cout << line << std::endl;
        } else if((i+2)% 5== 0){
            //std::cout << line << std::endl;
        } else if((i+1)% 5== 0){
            //std::cout << line << std::endl;
            mapNodes[m] = new node(n,d);
            m++;
        }
        i++;
    }
    //std::cout << "closing file...";
    input1.close();
    //std::cout << "file closed" << std::endl;

    //read in all the actions
    std::ifstream input2("Actions.txt");
    std::string type;
    std::string id;
    std::string name;
    std::string desc;
    node* temp;
    action* tempPush;
    int g = 0;
    i=0;
    m=0;
    //std::cout << "loading actions...";
    for(std::string line;getline(input2,line); ){
        //std::cout << line << std::endl;
        if(line == "generic action"){
            type = line;
            for(int x = 0;x<4;x++){
                getline(input2,line);
                //std::cout << line << std::endl;
                if((x+4)%4 == 0){
                    id = line;
                }else if((x+3)%4 == 0){
                    name = line;
                }else if((x+2)%4 == 0){
                    desc = line;
                }else if((x+1)%4 == 0){
                    tempPush = new action(type,id,name,desc);
                    actionList.push_back(tempPush);
                    tempPush = 0;
                    m++;
                }
            }
        } else if(line == "move action"){
            type = line;
            for(int x = 0;x<5;x++){
                getline(input2,line);
                //std::cout << line << std::endl;
                if((x+5)%5 == 0){
                    id = line;
                }else if((x+4)%5 == 0){
                    name = line;
                }else if((x+3)%5 == 0){
                    desc = line;
                }else if((x+2)%5 == 0){
                    temp = getNode(mapNodes,line);
                }else if((x+1)%5 == 0){
                    tempPush = new moveAction(type,id,name,desc,temp);
                    actionList.push_back(tempPush);
                    tempPush = 0;
                    m++;
                }
            }
        } else if(line == "pickup action"){
            type = line;
            for(int x = 0;x<4;x++){
                getline(input2,line);
                //std::cout << line << std::endl;
                if((x+4)%4 == 0){
                    id = line;
                }else if((x+3)%4 == 0){
                    name = line;
                }else if((x+2)%4 == 0){
                    desc = line;
                }else if((x+1)%4 == 0){
                    tempPush = new pickupAction(type,id,name,desc);
                    actionList.push_back(tempPush);
                    //std::cout << "LOADING PICKUP ACTION"<<tempPush->getName()<<std::endl;
                    tempPush = 0;
                    m++;
                }
            }
        } else if(line == "key action"){
            type = line;
            for(int x = 0;x<5;x++){
                getline(input2,line);
                //std::cout << line << std::endl;
                if((x+2)%5 == 0){
                    std::istringstream convert(line);
                    convert >> g;
                    //std::cout<<g;
                }else if((x+5)%5 == 0){
                    id = line;
                }else if((x+4)%5 == 0){
                    name = line;
                }else if((x+3)%5 == 0){
                    desc = line;
                }else if((x+1)%5 == 0){
                    tempPush = new keyAction(type,id,name,desc,g);
                    actionList.push_back(tempPush);
                    tempPush = 0;
                    m++;
                }
            }
        } else if(line == "use action"){
            type = line;
            for(int x = 0;x<6;x++){
                getline(input2,line);
                //std::cout << line << std::endl;
                if((x+6)%6 == 0){
                    id = line;
                }else if((x+5)%6 == 0){
                    name = line;
                }else if((x+4)%6 == 0){
                    desc = line;
                }else if((x+3)%6 == 0){
                    temp = getNode(mapNodes,line);
                }else if((x+2)%6 == 0){
                    std::istringstream convert(line);
                    convert >> g;
                }else if((x+1)%6 == 0){
                    tempPush = new useAction(type,id,name,desc,g,temp);
                    actionList.push_back(tempPush);
                    tempPush = 0;
                    m++;
                }
            }
        }
    }
    input2.close();
    //std::cout << "actions loaded.";

    //read in all the items
    std::ifstream input3("Items.txt");
    action* u;
    action* it;
    item* tempItem;
    i=0;
    m=0;
    //std::cout << "Loading items..." <<std::endl;
    for(std::string line;getline(input3,line); ){
        //std::cout << line << std::endl;
        if((i+4)% 4 == 0){
            name = line;
        }else if((i+3)% 4 == 0){
            u = getAction(actionList,line);
            //std::cout << u->getID();
        }else if((i+2)% 4 == 0){
            it = getAction(actionList,line);
            //std::cout << it->getID();
        }else if((i+1)% 4 == 0){
            //std::cout << "blah3";
            tempItem = new item(name,u,it);
            itemList.push_back(tempItem);
            static_cast<pickupAction*>(it)->setPickup(itemList[m]);
            //std::cout << "item loaded: "<<itemList[m]->getName() + " ";
            u = 0;
            //std::cout << "item use: " <<itemList[m]->getUse()->getID() + " ";
            tempPush = 0;
            m++;
        }
        i++;
    }
    input3.close();
    //std::cout << " ...items loaded.";

    //put all the actions and items into their respective nodes
    std::ifstream input4("Nodes.txt");
    node* cnode;
    std::vector<std::string> vect;
    i=0;
    m=0;
    //std::cout << "loading items and actions into nodes..." << std::endl;
    for(std::string line;getline(input4,line); ){
        if((i+5)% 5== 0){
            cnode = getNode(mapNodes,line);
            //std::cout << cnode->getName() << std::endl;
        } else if((i+4)% 5== 0){
            //std::cout << line << std::endl;
        } else if((i+3)% 5== 0){
            //std::cout << line << std::endl;
            std::stringstream ss(line);
            std::string next;
            while(ss.good()){
                getline(ss,next,',');
                vect.push_back(next);
                //std::cout << next << " ";
            }
            //std::cout << "next"<<std::endl;
            for(int j=0;j<int(vect.size());j++){
                //std::cout << "next"<<std::endl;
                //std::cout << vect[j] << " " << getItem(itemList,vect[j])->getName() << "|";
                if(vect[j]!="0"){
                    cnode->addItem(getItem(itemList,vect[j]));
                }
            }
            //std::cout << std::endl;
            vect.clear();
        } else if((i+2)% 5== 0){
            //std::cout << line << std::endl;
            std::stringstream ss(line);
            std::string next;
            while(ss.good()){
                getline(ss,next,',');
                vect.push_back(next);
                //std::cout << next << std::endl;
            }
            //std::cout << std::endl;
            for(int j=0;j<int(vect.size());j++){
                //std::cout << "next"<<std::endl;
                //std::cout << vect[j] << std::endl;
                //std::cout << " " << this->getAction(actionList,vect[j])->getName() << "|";
                if(vect[j]!="0"){
                    cnode->addAction(this->getAction(actionList,vect[j]));
                }
            }
            vect.clear();
        } else if((i+1)% 5== 0){
            //std::cout << line << std::endl;
        }
        i++;
    }
    input4.close();
    //std::cout << "...loaded." << std::endl << std::endl;
    cnode = 0;
    temp = 0;
    delete cnode;
    delete temp;

    std::cout << "You are a member of a Terran freighter crew tasked with transporting munitions to Raynor's Raiders. Usually you wait out the trip in cryo-stasis while the ship's AI handles the flying. But this time your trip isn't so peaceful..."<<std::endl<<std::endl;
    std::string newname;
    std::cout << "What is your name?" << std::endl;
    std::cin >> newname;
    std::cout << std::endl << std::endl;
    std::cout << mapNodes[0]->getDesc() << std::endl;
    user = new player(newname,mapNodes[0]);
    std::cout << "Slowly the door to your compartment opens. You slowly sit up from your long sleep as your thawed body recovers its strength. You notice that none of the other compartments have opened yet." <<std::endl;
}

void Map::completeGoal(int gnum, action* input){
    //std::cout << "goal completed: " << gnum;
    if(gnum == 99){
        std::cout << input->getDesc() <<std::endl;
    }else if(gnum == 0 and goals[0] ==false){
        mapNodes[0]->removeAction("Locked door");
        //std::cout << actionList.size();
        mapNodes[0]->addAction(this->getAction(actionList,"Move to Hub"));
        //std::cout << this->getAction(actionList,"Move to Hub")->getID() << this->getAction(actionList,"Move to Hub")->getDesc() << static_cast<moveAction*>(this->getAction(actionList,"Move to Hub"))->getMove()->getDesc();
        goals[0] = true;
        //user->getInventory()->getItem("COMM")->changeUse(this->getAction(actionList,"Use COMM2"));
        mapNodes[0]->setDesc("The cryo-stasis chamber is cold and your buddies still are stuck in their compartments.");
        std::cout << input->getDesc() <<std::endl;
        input->setDesc("\"The manual backup should be kept somewhere in the Maintenance Bay. Try not to break anything while looking around. Once you have it come slot it in up here in the AI core.\"");
    }else if(gnum == 0){
        std::cout << input->getDesc() <<std::endl;
    }else if(gnum == 1){
        user->getInventory()->removeItem("Manual Backup");
        this->getAction(actionList,"Use COMM")->setDesc("Go get the parts for the missile from storage and then assemble them in the Maintenance Bay. You'll need a thruster, a shell, and a payload.");
        goals[1] = true;
        mapNodes[3]->removeAction("Read screen");
        mapNodes[3]->addAction(this->getAction(actionList,"Check AI"));
        std::cout << input->getDesc() <<std::endl;
    }else if(gnum == 2 and (user->getInventory()->checkInventory("Use Missile Shell")->getID()!="err") and (user->getInventory()->checkInventory("Use Nuclear Warhead")->getID()!="err") and (user->getInventory()->checkInventory("Use Rocket Thruster")->getID()!="err") and goals[1]==true){
        user->getInventory()->removeItem("Rocket Thruster");
        user->getInventory()->removeItem("Missile Shell");
        user->getInventory()->removeItem("Nuclear Warhead");
        user->getInventory()->addItem(getItem(itemList,"Missile"));
        this->getAction(actionList,"Use COMM")->setDesc("What are you waiting for? USE THE MISSILE.");
        std::cout << input->getDesc() <<std::endl;
        goals[2] = true;
        mapNodes[1]->removeAction(input->getName());
    }else if(gnum == 2 and goals[1]==false){
        std::cout << "Your COMM squawks at you, \"What are you trying to do? I need you to bring me that backup!\"" << std::endl;
    }else if(gnum == 2){
        std::cout << "You are missing one of the components to the missile. You need a payload, shell, and thruster." << std::endl;
        std::cout << user->getInventory()->checkInventory("Use Missile Shell")->getID() << " " << user->getInventory()->checkInventory("Use Nuclear Warhead")->getID() << " " << user->getInventory()->checkInventory("Use Rocket Thruster")->getID() << std::endl;
    }else if(gnum == 3){
        user->getInventory()->removeItem("Missile");
        goals[3] = true;
        this->getAction(actionList,"Use COMM")->setDesc("Get to the Engine Room and get us moving. Don't just go pulling handles randomly though, read the manual first. Has to be around here somewhere.");
        std::cout << input->getDesc() <<std::endl;
    }else if((gnum == 4 or gnum == 5 or gnum == 6 or gnum == 7) and goals[1]==false){
        std::cout << "Your COMM squawks at you, \"What are you doing in the engine room? Bring me that backup already.\"" << std::endl;
    }else if((gnum == 4 or gnum == 5 or gnum == 6 or gnum == 7) and goals[2]==false){
        std::cout << "Your COMM squawks at you, \"What are you doing in the engine room? You've got a missile to put together.\"" << std::endl;
    }else if((gnum == 4 or gnum == 5 or gnum == 6 or gnum == 7) and goals[3]==false){
        std::cout << "Your COMM squawks at you, \"Unless you want to dump the missile into the engine and kill us all, I suggest you get back to the Maintenance Bay and FIRE THAT MISSILE.\"" << std::endl;
    }else if(gnum == 6){
        if(goals[6] == false){
            goals[6] = true;
        }else{
            std::cout << "Your mistake creates a chain reaction in the engine room and everything explodes. The fate of the ship is a mystery to you, but at least you're not an SCV pilot anymore." << std::endl;
            goals[8]=true;
        }
        //std::cout << goals[4]<<goals[5]<<goals[6]<<goals[7];
        std::cout << input->getDesc() <<std::endl;
    }else if(gnum == 4){
        if(goals[6]==true and goals[4] == false){
            goals[4] = true;
            std::cout << input->getDesc() <<std::endl;
        } else{
            std::cout << "Your mistake creates a chain reaction in the engine room and everything explodes. The fate of the ship is a mystery to you, but at least you're not an SCV pilot anymore." << std::endl;
            goals[8]=true;
        }
        //std::cout << goals[4]<<goals[5]<<goals[6]<<goals[7];

    }else if(gnum == 5){
        if(goals[6]==true and goals[4] == true and goals[5] == false){
            goals[5] = true;
            std::cout << input->getDesc() <<std::endl;
        } else{
            std::cout << "Your mistake creates a chain reaction in the engine room and everything explodes. The fate of the ship is a mystery to you, but at least you're not an SCV pilot anymore." << std::endl;
            goals[8]=true;
        }
        //std::cout << goals[4]<<goals[5]<<goals[6]<<goals[7];

    }else if(gnum == 7){
        if(goals[4]==true and goals[5] == true and goals[6] == true and goals[7] == false){
            goals[7] = true;
            mapNodes[5]->removeAction("Check navigation");
            mapNodes[5]->addAction(this->getAction(actionList,"Use navigation"));
            //std::cout << goals[4]<<goals[5]<<goals[6]<<goals[7];
            std::cout << input->getDesc() <<std::endl;
            this->getAction(actionList,"Use COMM")->setDesc("Good job. Now just go up to the bridge and authorize me to navigate us the hell out of here.");
            std::cout << "The Engine room lights up and starts to hum. Sounds like things are working again. You give yourself a pat on the back for not blowing yourself up today." << std::endl;
        } else{
            std::cout << "Your mistake creates a chain reaction in the engine room and everything explodes. The fate of the ship is a mystery to you, but at least you're not an SCV pilot anymore." << std::endl;
            goals[8]=true;
        }
    }else if(gnum == 8){
        std::cout << input->getDesc() <<std::endl;
        std::cout << "As you input the coordinates into the navigation console your ship begins to move. PRATHER orients the ship in the direction that you have indicated and blasts off at a hyper speed. Feeling a sense of accomplishment and fatigue you head back to the Cryo-Station. You remove yourself from your SCV and open up your compartment for sleep. The door closes around you and you fall back asleep to awaken when you reach your destination."<< std::endl << "You win!";
        goals[8] = true;
    }
}

void Map::play(){
    while(this->getGoal(8) == false){
        std::cout << std::endl;
        user->getLocation()->getActions(std::cout);
        std::string nextAction = "";
        std::cout << ">>";
        std::cin.clear();
        std::cin.sync();
        std::getline(std::cin,nextAction);
        //std::cout << nextAction;
        action* next = user->getInventory()->checkInventory(nextAction);
        //std::cout << next->getID();
        if(next->getName() == "err"){
            next = this->getAction(user->getLocation()->getActionList(),nextAction);
            //std::cout << next->getID();
        }
        if(nextAction == "Check bag"){
            user->getInventory()->getInventory(std::cout);
        }else if(next->getType() == "generic action" ){
            std::cout << next->getDesc() << std::endl;
        }else if(next->getType() == "key action"){
            static_cast<keyAction*>(next)->finishGoal(user->getLocation(),this);
        }else if(next->getType() == "move action"){
            static_cast<moveAction*>(next)->moveToNode(user);
        }else if(next->getType() == "use action"){
            static_cast<useAction*>(next)->use(user,this);
        }else if(next->getType() == "pickup action"){
            static_cast<pickupAction*>(next)->pickup(user->getLocation(),user);
        }
        //delete next;
    }

}
