#include <iostream>
#include <vector>


//The core of the game.
//The generic class serves as the template for the actions that can be performed.
//Can also create generic actions that essentially do nothing but provide descriptive text.
//Such actions would primarily be useful when the player does not have necessary goals or items to do something yet.
class action{
    public:
        action(std::string Atype, std::string AID, std::string Aname,std::string Adesc):actionType(Atype),actionID(AID),actionName(Aname),actionDesc(Adesc){};
        ~action(){};

        std::string getName()const{return actionName;};
        std::string getDesc()const{return actionDesc;};
        void setDesc(std::string input){actionDesc = input;};
        std::string getType()const{return actionType;};
        std::string getID()const{return actionID;};;

    private:
        std::string actionType;
        std::string actionID;
        std::string actionName;
        std::string actionDesc;
};

//items is sort of a go between for use actions and the inventory
//this allows actions to be switched out for certain items with multiple functions (particularly the AI)
class item{
    public:
        item(std::string item,action* useOf,action* pick):itemName(item),use(useOf),pickup(pick){};
        ~item(){delete use;};

        action* getUse(){return use;};
        void changeUse(action* newUse){use = newUse;};
        std::string getName(){return itemName;};

    private:
        std::string itemName;
        action* use;
        action* pickup;
};

//when a node is created, all the actions associated with that node are set.
//player can only take actions in current node
//actions may be changed by the Map
class node{
    public:
        node(std::string n,std::string desc):name(n),description(desc){};
        ~node(){};
        std::string getName()const{return name;};
        std::string getDesc()const{return description;};
        void setDesc(std::string input){description = input;};
        std::vector<action*>& getActionList(){return nodeActions;};
        void getActions(std::ostream& out)const;
        void addAction(action* input);
        void addItem(item* input);
        void removeAction(std::string actionName);
        void removeItem(std::string itemName);
        //It is assumed that the action or item is in the node, when removal is being called.

    private:
        std::string name;
        std::string description;
        std::vector<action*> nodeActions;
        std::vector<item*> itemPickups;
};

//inventory belongs to the player which tracks and retrieves the items they've picked up.
//inventory will be created after the first key action where the player activates the AI.
class inventory{
    public:
        inventory(){errorHandler = new action("generic action","err","err","You cannot perform that action here.");};
        ~inventory(){};

        void getInventory(std::ostream& out) const;
        item* getItem(std::string itemName);
        void addItem(item* itemAdded);
        void removeItem(std::string itemName);
        action* checkInventory(std::string input);


    private:
        std::vector<item*> itemsInventory;
        action* errorHandler;

};

//player primarily just stores the current location and player inputted name.
//Currently does not have an inventory. May be added later.
class player{
    public:
        player(std::string pN,node* cNode):Pname(pN),currentNode(cNode){playerInventory = new inventory();};
        ~player(){delete playerInventory;};

        std::string name()const{return Pname;};
        node* getLocation(){return currentNode;};
        inventory* getInventory(){return playerInventory;};
        void name(std::string input) {Pname = input;};
        void setNode(node* input){currentNode = input;};

    private:
        std::string Pname;
        node* currentNode;
        inventory* playerInventory;
};

//Our singleton Map that when constructed,
//instantiates all the nodes that make up the rooms of our game
//essentially is a factory for all the nodes in the game.
class Map{
    public:
        Map();
        ~Map(){delete [] goals; delete [] mapNodes;};

        bool& operator[](int n){return goals[n];}

        bool getGoal(int i)const{return goals[i];}
        void completeGoal(int gnum, action* input);//when goal is completed, Map checks goals' current state to determine any changes to nodes/actions available.
        void play();
        action* getAction(std::vector<action*>& actionL,std::string n);
        item* getItem(std::vector<item*>& itemL,std::string n);
        node* getNode(node** nodeList,std::string n);

    private:
        bool* goals;
        node** mapNodes;
        std::vector<item*> itemList;
        player* user;
        action* errorHandler;
        std::vector<action*> actionList;

};

//keyActions are actions that complete goals, setting Map's goals to true.
class keyAction : public action{
    public:
        keyAction(std::string Atype, std::string AID, std::string Aname,std::string Adesc, int goal):action(Atype,AID,Aname,Adesc),goalNum(goal){};
        ~keyAction(){};

        void finishGoal(node* location, Map* getMap);
        //flips goal to true in Map array(calling Map's completeGoal), throws up description for action, remove self from node actions.

    private:
        int goalNum;
};

//moveActions are actions that change the node the player is in.
class moveAction : public action{
    public:
        moveAction(std::string Atype, std::string AID, std::string Aname,std::string Adesc,node* location):action(Atype,AID,Aname,Adesc),moveTo(location){};
        ~moveAction(){moveTo = 0;};

        void moveToNode(player* self){self->setNode(moveTo); std::cout << this->getDesc() << std::endl << moveTo->getDesc() << std::endl;};
        node* getMove()const{return moveTo;};
        //changes node(calls node setter from player) and then prints the description and actions for the new node/movement.

    private:
        node* moveTo;
};

//pickup actions are for when an item needs to be picked up from a node and transferred to the player's inventory
class pickupAction : public action{
    public:
        pickupAction(std::string Atype, std::string AID, std::string Aname,std::string Adesc):action(Atype,AID,Aname,Adesc){};
        ~pickupAction(){};

        void setPickup(item* input){pickedUp = input;};

        void pickup(node* removeFrom,player* self);
        //removes self and item from node, adds item to inventory, prints description for action.
    private:
        item* pickedUp;
};

//use actions for the most part merely fulfill a goal (which often results in the item being removed from the inventory)
//An exception exists for AI, which stays in the inventory and merely prints out current goals for the player.
class useAction : public action{
    public:
        useAction(std::string Atype, std::string AID, std::string Aname,std::string Adesc,int goal,node* loc):action(Atype,AID,Aname,Adesc),goalNum(goal),useLocation(loc){};
        ~useAction(){useLocation = 0;};

        void use(player* self, Map* getMap);
        //flip goal to true if player location matches useLocation

    private:
        int goalNum;
        node* useLocation;
 };

