
#include "study_in_pink2.h"
const Position Position::npos = Position(-1, -1);
/*
 * CLASS: MapElement
 */
MapElement::MapElement(ElementType in_type)
{
     type=in_type;
}

MapElement::~MapElement() {}

ElementType MapElement::getType() const
{
    return type;
}
/*
 * CLASS: Path
 */
Path::Path()
    : MapElement(PATH) {
        this->type=PATH;
    }
/*
 * CLASS: Wall
 */
Wall::Wall()
    : MapElement(WALL) {
        this->type=WALL;
    }
/*
 * CLASS: FakeWall
 */
FakeWall::FakeWall(int in_req_exp)
    : MapElement(ElementType::FAKE_WALL)
{
    req_exp=in_req_exp;
}

int FakeWall::getReqExp() const
{
    return req_exp;
}

void FakeWall::setReqExp(int req_exp)
{
    this->req_exp=req_exp;
}
/*
 * CLASS: Position
 */
static const Position npos = Position(-1, -1);
Position::Position(int r, int c)
{
    this->r=r;
    this->c=c;
}

Position::Position(const string &str_pos)
{
    r=stoi(str_pos.substr(1,str_pos.find(',')-1));
    c=stoi(str_pos.substr(str_pos.find(',')+1,str_pos.find(')')-3));
}

int Position::getRow() const
{
    return r;
}

int Position::getCol() const
{
    return c;
}

void Position::setRow(int r)
{
    this->r=r;
}

void Position::setCol(int c)
{
    this->c=c;
}

string Position::str() const
{
    string a,b;
        a=to_string(r);
        b=to_string(c);
        string temp;

        temp+="(";
        temp+=a;
        temp+=",";
        temp+=b;
        temp+=")";
        return temp ;
}

bool Position::isEqual(Position position) const
{
    if(position.r==r && position.c==c) return true;
    else return false;
}
bool Position::isEqual(int r, int c) const
{
    if(this->r==r && this->c==c) return true;
    else return false;
}
/*
 * CLASS: MovingObject
 */
MovingObject::MovingObject(int index, const Position pos, Map *map, const string &name)
{
    this->index=index;
    this->pos=pos;
    this->map=map;
    this->name=name;
}

Position MovingObject::getCurrentPosition() const
{
    return pos;
}

MovingObject::~MovingObject() {}
/*
 * CLASS: Character 
 */
Character::Character(int index, const Position pos, Map *map, const string &name)
    : MovingObject(index,pos,map,name)
{
    this->index=index;
    this->pos=pos;
    this->map=map;
    this->name=name;
}

/*
 * CLASS: Sherlock 
 */

Sherlock::Sherlock(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp)
    : Character(index,init_pos,map,"Sherlock")
{
    this->index=index;
    this->moving_rule=moving_rule;
    this->pos=init_pos;
    this->map=map;
    this->hp=init_hp;
    this->exp=init_exp;
    this->index_moving_rule=0;
    bag= new SherlockBag(this);
}

Position Sherlock::getNextPosition()
{
    if(index_moving_rule==moving_rule.length()){
        index_moving_rule=0;
    }
    int n=moving_rule.length();

    Position current_position = pos;
    char next_move = moving_rule[index_moving_rule];
    if(next_move=='U'){
        current_position.setRow(current_position.getRow()-1);
    }
    else if(next_move=='D'){
        current_position.setRow(current_position.getRow()+1);
    }
    else if(next_move=='L'){
        current_position.setCol(current_position.getCol()-1);
    }
    else if(next_move=='R'){
        current_position.setCol(current_position.getCol()+1);
    }
    index_moving_rule++;   
    if(map->isValid(current_position,this)==false){
        current_position.setRow(-1);
        current_position.setCol(-1);
    }
    return current_position;
}

void Sherlock::move()
{
    Position a;
        a= getNextPosition();
        if(exp!=0){
        if(a.getCol()!=-1 && a.getRow()!=-1){
            pos.setCol(a.getCol());
            pos.setRow(a.getRow());
        }
        }
}

string Sherlock::str() const
{
     string x,y,z;
        x=to_string(index);
        y=pos.str();
        z=moving_rule;

        char final[100]; 
        sprintf(final, "Sherlock[index=%s;pos=%s;moving_rule=%s]", x.c_str(), y.c_str(), z.c_str());
        return final;
}

MovingObjectType Sherlock::getObjectType() const
{
    return MovingObjectType::SHERLOCK;
}

int Sherlock::getHP() const{
    if(hp>500) return 500;
    else if(hp<0) return 0;
    else return hp;
}
int Sherlock::getEXP() const
{
    if(exp<0) return 0;
    else if(exp>900) return 900;
    else return exp;
    return exp;
}
void Sherlock::setHP(int hp)
{
    if(hp>500) this->hp=500;
    else if(hp<0) this->hp=0;
    else this->hp=hp;
}

void Sherlock::setEXP(int exp)
{   
    if(exp>900) this->exp=900;
    else if(exp<0) this->exp=0;
    else this->exp=exp;
}
void Sherlock::setPos(Position pos)
{
    this->pos.setRow(pos.getRow());
    this->pos.setCol(pos.getCol());
    
}

bool Sherlock::meet(RobotS *robotS)
{
    int a;
    a=getEXP();
    bool result;
    if(a>=400){
        if(bag->isFull()==false) {
        bag->insert(robotS->NewItem());
        }
        result= true;
    }
    else if(a<400)
    {
        if(bag->checkItem(EXCEMPTION_CARD)==true){
            BaseItem* card =bag->get(EXCEMPTION_CARD);
            card->use(this,nullptr);
            result= false;
        }
        else if(bag->checkItem(EXCEMPTION_CARD)==false){
            setEXP(ceil(getEXP()-0.1*getEXP()));
            result= false;
        }
    }
    //cout<<getEXP()<<endl;
    //cout<<getHP()<<endl;
   // cout<<bag->str()<<endl;
   BaseItem* item = bag->get();
    if (item!=nullptr ){
        item->use(this, nullptr);
    }
   
    return result;
}

bool Sherlock::meet(RobotW *robotW)
{   
    bool result;
    if(bag->isFull()==false) {
        bag->insert(robotW->NewItem());
    }
    BaseItem* item = bag->get();
    if (item!=nullptr ){
        item->use(this, nullptr);
    }
    result= true;
    return result;
}

bool Sherlock::meet(RobotSW *robotSW)
{
    bool result;
    if(getEXP()>=300 && getHP() >=335) {
        if(bag->isFull()==false) {
            bag->insert(robotSW->NewItem());
        }
        result= true;
    }
    else if(getEXP()<300 || getHP()<335){
        if(bag->checkItem(EXCEMPTION_CARD)==true){
            BaseItem* card =bag->get(EXCEMPTION_CARD);
            card->use(this,nullptr);
            result= false;
        }
        else if(bag->checkItem(EXCEMPTION_CARD)==false){
            setHP(ceil(getHP()-0.15*getHP()));
            setEXP(ceil(getEXP()-0.15*getEXP()));
            result = false;
        }
    } 
    
    BaseItem* item = bag->get();
    if (item!=nullptr ){
        item->use(this, nullptr);
    }
    
    return result;
}
bool Sherlock::meet(RobotC *robotC)
{   
    bool result;
    if(getEXP()>=500){
        Position pos=robotC->getNextPosition();
        setPos(pos);
        result= true;
    }
    else if(getEXP()<500){
        if(bag->isFull()==false) {
        bag->insert(robotC->NewItem());
        }
        result= false;
    }
    BaseItem* item = bag->get();
    if (item!=nullptr ){
        item->use(this, nullptr);
    }
    return result;
}

bool Sherlock::meet(Watson *watson)
{
   
   if(bag->checkItem(PASSING_CARD)==false || watson->getBag()->checkItem(EXCEMPTION_CARD)==false)
   return false;
   else{
    while(bag->get(PASSING_CARD)!=nullptr){
        if(watson->getBag()->isFull()==false) watson->getBag()->insert(bag->get(PASSING_CARD));
    }

    while(watson->getBag()->get(EXCEMPTION_CARD)!=nullptr){
        if(bag->isFull()==false) bag->insert(watson->getBag()->get(EXCEMPTION_CARD));
    }
    return true;
    }
}

BaseBag *Sherlock::getBag() const
{
    return bag;
}
Sherlock::~Sherlock() {}
/*
 * CLASS: Watson 
 */

Watson::Watson(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp)
    : Character(index,init_pos,map,"Watson")
{
    this->index=index;
    this->moving_rule=moving_rule;
    this->pos=init_pos;
    this->map=map;
    this->hp=init_hp;
    this->exp=init_exp;
    this->index_moving_rule=0;
    this->moving_rule_temp=moving_rule;
    bag= new WatsonBag(this);   
}

Position Watson::getNextPosition()
{   
    if(index_moving_rule==moving_rule.length()){
        index_moving_rule=0;
    }
    int n=moving_rule.length();

    Position current_position = pos;
    char next_move = moving_rule[index_moving_rule];
    if(next_move=='U'){
        current_position.setRow(current_position.getRow()-1);       
    }
    else if(next_move=='D'){
        current_position.setRow(current_position.getRow()+1);
    }
    else if(next_move=='L'){
        current_position.setCol(current_position.getCol()-1);
    }
    else if(next_move=='R'){
        current_position.setCol(current_position.getCol()+1);
    }
       
    if(map->isValid(current_position,this)==false){
        
        current_position.setRow(-1);
        current_position.setCol(-1);
    }
    index_moving_rule++;
    return current_position;
}

void Watson::move()
{
    
    Position a;
    a= getNextPosition();
    if(exp!=0){
    if(a.getCol()!=-1 && a.getRow()!=-1){
        pos.setCol(a.getCol());
        pos.setRow(a.getRow());
    }
    }
}

string Watson::str() const
{
    string x,y,z;
        x=to_string(index);
        y=pos.str();
        z=moving_rule;

        char final[100]; 
        sprintf(final, "Watson[index=%s;pos=%s;moving_rule=%s]", x.c_str(), y.c_str(), z.c_str());
        return final;
}

MovingObjectType Watson::getObjectType() const
{
    return MovingObjectType::WATSON;
}

int Watson::getHP() const
{
    if(hp>500) return 500;
    else if(hp<0) return 0;
    else return hp;
}

int Watson::getEXP() const
{
    if(exp<0) return 0;
    else if(exp>900) return 900;
    else return exp;
}

void Watson::setHP(int hp)
{
    if(hp>500) this->hp=500;
    else if(hp<0) this->hp=0;
    else this->hp=hp;
}

void Watson::setEXP(int exp)
{
    if(exp>900) this->exp=900;
    else if(exp<0) this->exp=0;
    else this->exp=exp;
}

bool Watson::meet(RobotS *robotS)
{
    return true;
}

bool Watson::meet(RobotW *robotW)
{   
    bool result;
    
   if(getHP()>=350){
     if(bag->checkItem(PASSING_CARD)==true){
        BaseItem* card =bag->get(PASSING_CARD);
        card->use(this,robotW);}
        if(bag->isFull()==false) {
         bag->insert(robotW->NewItem()); }
        result= true;   
        
   } else if(getHP()<350){
        if(bag->checkItem(PASSING_CARD)==true){
        BaseItem* card =bag->get(PASSING_CARD);
        card->use(this,robotW);
        bag->insert(robotW->NewItem());
        result= true;
        }else {
        setHP(ceil(getHP()-0.05*getHP()));
        result= false;
        }    
    }

    BaseItem* item = bag->get();
    if (item!=nullptr ){
        item->use(this, nullptr);
    }
    
    return result;
}

bool Watson::meet(RobotSW *robotSW)
{
    bool result;
    if(bag->checkItem(PASSING_CARD)==true){
            BaseItem* card =bag->get(PASSING_CARD);
            card->use(this,robotSW);
            bag->insert(robotSW->NewItem());
            result= true;
    }
    else{
   if(getEXP()>=600 && getHP()>=165){
    if(bag->isFull()==false) {
         bag->insert(robotSW->NewItem());  
    }
    result= true;
   }
   else if(getEXP()<600 || getHP()<165){
        setHP(ceil(getHP()-0.15*getHP()));
        setEXP(ceil(getEXP()-0.15*getEXP()));
        result= false;  
    }
    BaseItem* item = bag->get();
    if (item!=nullptr ){
        item->use(this, nullptr);
    }
    }
    return result;
}

bool Watson::meet(RobotC *robotC)
{
    bool result;
    if(bag->checkItem(PASSING_CARD)==true){
            BaseItem* card =bag->get(PASSING_CARD);
            card->use(this,robotC);
            result= true;
    }
    if(bag->isFull()==false) {
         bag->insert(robotC->NewItem()); 
            result= true;
    }
    BaseItem* item = bag->get();
    if (item!=nullptr ){
        item->use(this, nullptr);
    }
    return result;
}

bool Watson::meet(Sherlock *sherlock)
{
    if(sherlock->getBag()->checkItem(PASSING_CARD)==false || bag->checkItem(EXCEMPTION_CARD)==false)
   return false;
   else{
    while(sherlock->getBag()->get(PASSING_CARD)!=nullptr){
        if (bag->isFull()==false) bag->insert(sherlock->getBag()->get(PASSING_CARD));
    }

    while(bag->get(EXCEMPTION_CARD)!=nullptr){
        if(sherlock->getBag()->isFull()==false) sherlock->getBag()->insert(bag->get(EXCEMPTION_CARD));
    }
    return true;
    }
}

BaseBag *Watson::getBag() const
{
    return bag;
}
Watson::~Watson() {}
/*
 * CLASS: Map
 */
Map::Map(int num_rows, int num_cols, int num_walls, Position *array_walls, int num_fake_walls, Position *array_fake_walls)
    {
        this->num_rows=num_rows;
        this->num_cols=num_cols;
        

       map=new MapElement**[num_rows];
       for(int i=0;i<num_rows;i++){
           map[i]=new MapElement*[num_cols];
           for(int j=0;j<num_cols;j++){
                map[i][j]=new Path();
                for(int k=0;k<num_walls;k++){
                    if(array_walls[k].getRow()==i && array_walls[k].getCol()==j){
                        map[i][j]=new Wall();
                        break;
                    }
                }
                if(map[i][j]->getType()!=WALL){
                for(int k=0;k<num_fake_walls;k++){
                    if(array_fake_walls[k].getRow()==i && array_fake_walls[k].getCol()==j){
                        map[i][j]=new FakeWall(0);
                        ((FakeWall*)map[i][j])->setReqExp((i*257+j*139 +89)%900 +1);
                        break;
                    }   
                }
                }
            }
        }
    }
Map::~Map()
{
    delete map;
}

int Map::getNumRows() const
{
    return num_rows;
}

int Map::getNumCols() const
{
    return num_cols;
}

ElementType Map::getElementType(int i, int j) const
{
    return map[i][j]->getType();
}

bool Map::isValid(const Position &pos, MovingObject *mv_obj) const
{   
    bool result=true;
    if(pos.getRow()<0 || pos.getRow()>=num_rows || pos.getCol()<0 || pos.getCol()>=num_cols) return false;
    
    ElementType temp;
    temp=map[pos.getRow()][pos.getCol()]->getType();

    if(temp==WALL) return false;
    if(temp==PATH) return true;
    
    if(mv_obj->getObjectType()== SHERLOCK || mv_obj->getObjectType()== CRIMINAL || mv_obj->getObjectType()==ROBOT) return true;
    else if(mv_obj->getObjectType()== WATSON){
            if(((FakeWall*)map[pos.getRow()][pos.getCol()])->getReqExp()<=((Watson*)mv_obj)->getEXP()) return true;
            else return false;
        }     
    return result; 
}
/*
 * CLASS: Criminal 
 */

Criminal::Criminal(int index, const Position &init_pos, Map *map, Sherlock *sherlock, Watson *watson)
    : Character(index,init_pos,map,"Criminal")
{
    this->index=index;
    this->pos=init_pos;
    this->map=map;
    this->sherlock=sherlock;
    this->watson=watson;
    this->count=0;
    
}

int Criminal::manhattanDistance(const Position &pos1, const Position &pos2) const
{
    int a, b;
        a=abs(pos1.getRow()-pos2.getRow());
        b=abs(pos1.getCol()-pos2.getCol());
        return a+b;
}

Position Criminal::getNextPosition()
{   
    
    Position current_position;
    
    Position sherlock_current_position = sherlock->getCurrentPosition();
    Position watson_current_position = watson->getCurrentPosition();

    Position adjacent_positions[4] = {
        Position(pos.getRow() - 1, pos.getCol()), 
        Position(pos.getRow(), pos.getCol() - 1), 
        Position(pos.getRow() + 1, pos.getCol()), 
        Position(pos.getRow(), pos.getCol() + 1)  
    };

    int mat_up=-1;
    int mat_down=-1;
    int mat_left=-1;
    int mat_right=-1;

    if(map->isValid(adjacent_positions[0],this)){
        mat_up=manhattanDistance(adjacent_positions[0],sherlock_current_position)+manhattanDistance(adjacent_positions[0],watson_current_position);
    }
    if(map->isValid(adjacent_positions[1],this)){
        mat_left=manhattanDistance(adjacent_positions[1],sherlock_current_position)+manhattanDistance(adjacent_positions[1],watson_current_position);
    }
    if(map->isValid(adjacent_positions[2],this)){
        mat_down=manhattanDistance(adjacent_positions[2],sherlock_current_position)+manhattanDistance(adjacent_positions[2],watson_current_position);
    }
    if(map->isValid(adjacent_positions[3],this)){
        mat_right=manhattanDistance(adjacent_positions[3],sherlock_current_position)+manhattanDistance(adjacent_positions[3],watson_current_position);
    }

    if(mat_up==-1 && mat_down==-1 && mat_left==-1 && mat_right==-1) {
        current_position=pos;
    }
    else if(mat_up >= mat_right && mat_up >= mat_left && mat_up >= mat_down){
        current_position=adjacent_positions[0];
    }
    else if(mat_left >= mat_right && mat_left >= mat_down && mat_left >= mat_up){
        current_position=adjacent_positions[1];
    }
    else if(mat_down >= mat_up && mat_down >= mat_left && mat_down >= mat_right){
        current_position=adjacent_positions[2];
    }
    else if(mat_right >= mat_left && mat_right >= mat_up && mat_right >= mat_down){
        current_position=adjacent_positions[3];
    }
    
    return current_position;
}

void Criminal::move()
{
    Position a;
        a= getNextPosition();
        if(!a.isEqual(pos)){
        if(a.getCol()!=-1 && a.getRow()!=-1){
            prev_pos=pos;
            pos.setCol(a.getCol());
            pos.setRow(a.getRow());
            count++;
        }
        }
}

string Criminal::str() const
{
     string x,y;
        x=to_string(index);
        y=pos.str();

        char final[100]; 
        sprintf(final, "Criminal[index=%s;pos=%s]",x.c_str(),y.c_str());
        return final;
}

MovingObjectType Criminal::getObjectType() const
{
    return MovingObjectType::CRIMINAL;
}

int Criminal::getCount() const
{
    return count;

}

bool Criminal::isCreatedRobotNext() const
{
    if(count%3==0 && count>0) return true;
    else return false;
}

Position Criminal::getPrevPosition() const
{
    return prev_pos;
}
/*
 * CLASS: Robot 
 */
Robot::Robot(int index, const Position &pos, Map *map, Criminal *criminal, const string &name)
    : MovingObject(index,pos,map,name)
{
    this->index=index;
    this->pos=pos;
    this->map=map;
    this->robot_type=robot_type;
    this->criminal=criminal;

    int row=pos.getRow();
    int col=pos.getCol(); 
    int s;
    int p=row*col;

    if(p>=0 && p<=9) {
        s=p;
    }else{
   while (p >= 10) {
        s = 0;        
        while (p > 0) {
            s+= p % 10;
            p /= 10;
        }
        p = s; 
    }
    }
    if(s>=0 && s<=1) {
        this->item_= MAGIC_BOOK;
    }
    else if(s>=2 && s<=3) {
        this->item_= ENERGY_DRINK;
    }
    else if(s>=4 && s<=5) {
        this->item_= FIRST_AID;
    }
    else if(s>=6 && s<=7) {
        this->item_= EXCEMPTION_CARD;
    }
    else if(s>=8 && s<=9) {
        this->item_= PASSING_CARD;
    }

    this->item=NewItem();
}

BaseItem *Robot::NewItem()
{
   BaseItem *newItem=nullptr;
    if(item_==MAGIC_BOOK){
        newItem= new MagicBook();
    }
    else if(item_==ENERGY_DRINK){
        newItem= new EnergyDrink();
    }
    else if(item_==FIRST_AID){
        newItem= new FirstAid();
    }
    else if(item_==EXCEMPTION_CARD){
        newItem= new ExcemptionCard();
    }
    else if(item_==PASSING_CARD){
        newItem= new PassingCard(pos.getRow(),pos.getCol());
    }
    return newItem;
}

Robot *Robot::create(int index, Map *map, Criminal *criminal, Sherlock *sherlock, Watson *watson)
{
    if(criminal->getCount()==2) return new RobotC(index,criminal->getCurrentPosition(),map,criminal);
    
    if(criminal->manhattanDistance(criminal->getCurrentPosition(),sherlock->getCurrentPosition())<criminal->manhattanDistance(criminal->getCurrentPosition(),watson->getCurrentPosition()))
    return new RobotS(index,criminal->getCurrentPosition(),map,criminal,sherlock);
    if(criminal->manhattanDistance(criminal->getCurrentPosition(),sherlock->getCurrentPosition())>criminal->manhattanDistance(criminal->getCurrentPosition(),watson->getCurrentPosition()))
    return new RobotW(index,criminal->getCurrentPosition(),map,criminal,watson);
    
    return new RobotSW(index,criminal->getCurrentPosition(),map,criminal,sherlock,watson);

}

MovingObjectType Robot::getObjectType() const
{
    return MovingObjectType::ROBOT;
}

RobotType Robot::getType() 
{
    return robot_type;
}

Robot::~Robot() {}
/*
 *CLASS: RobotC 
 */
RobotC::RobotC(int index, const Position &init_pos, Map *map,  Criminal *criminal)
    : Robot(index,init_pos,map,criminal,"RobotC")
{
    this->index=index;
    this->pos=init_pos;
    this->map=map;
    this->criminal=criminal;
    this->robot_type=C;
}

int RobotC::getDistance(Sherlock *sherlock)
{
    int a;
    a=criminal->manhattanDistance(pos,sherlock->getCurrentPosition());
    return a;
}

int RobotC::getDistance(Watson *watson)
{
    int a;
    a=criminal->manhattanDistance(pos,watson->getCurrentPosition());
    return a;
}

Position RobotC::getNextPosition()
{
     return criminal->getCurrentPosition();
    
}

void RobotC::move()
{
    Position a;
    a= criminal->getPrevPosition();
    if(a.getCol()!=-1 && a.getRow()!=-1){
        pos.setCol(a.getCol());
        pos.setRow(a.getRow());
    }
}

string RobotC::str() const
{
    char final[1000];
    sprintf(final, "Robot[pos=%s;type=C;dist=]",pos.str().c_str());
   
    return final;
}

RobotType RobotC::getType() const
{
    return robot_type;
}

int RobotC::getDistance() const{
    return 0;
}

/*
 * CLASS: RobotW 
 */

RobotW::RobotW(int index, const Position &init_pos, Map *map, Criminal *criminal, Watson *watson)
    : Robot(index,init_pos,map,criminal,"RobotW")
{
    this->index=index;
    this->pos=init_pos;
    this->map=map;
    this->robot_type=W;
    this->criminal=criminal;
    this->watson=watson;
}

Position RobotW::getNextPosition()
{
    Position current_position = pos;
    Position watson_next_position = watson->getCurrentPosition();


    
    Position adjacent_positions[4] = {
        Position(current_position.getRow() - 1, current_position.getCol()),
        Position(current_position.getRow(), current_position.getCol() + 1),
        Position(current_position.getRow() + 1, current_position.getCol()),
        Position(current_position.getRow(), current_position.getCol() - 1)
    };

    int min_distance = 10000; 
    Position best_position = current_position;

    for (int i = 0; i < 4; ++i) {
        if (map->isValid(adjacent_positions[i], this)) {
            int distance = criminal->manhattanDistance(adjacent_positions[i], watson_next_position);
            if (distance < min_distance) {
                min_distance = distance;
                best_position = adjacent_positions[i];
            }
        }
    }

    return best_position;
   
}


void RobotW::move()
{
    Position a;
    a= getNextPosition();
    if(a.getCol()!=-1 && a.getRow()!=-1){
        pos.setCol(a.getCol());
        pos.setRow(a.getRow());
    }
}

string RobotW::str() const
{
    char final[1000];
    int a= getDistance();
    if(a==0) sprintf(final, "Robot[pos=%s;type=W;dist=0]",pos.str().c_str());
    else sprintf(final, "Robot[pos=%s;type=W;dist=%s]",pos.str().c_str(), to_string(a).c_str());
        return final;
}

RobotType RobotW::getType() const
{
    return robot_type;
}

int RobotW::getDistance() const
{
    int a;
    a=criminal->manhattanDistance(pos,watson->getCurrentPosition());
    return a;
}
/*
 * CLASS: RobotS 
 */
RobotS::RobotS(int index, const Position &init_pos, Map *map, Criminal *criminal, Sherlock *sherlock)
    : Robot(index,init_pos,map,criminal,"RobotS")
{
    this->index=index;
    this->pos=init_pos;
    this->map=map;
    this->robot_type=S;
    this->criminal=criminal;
    this->sherlock=sherlock;
}

Position RobotS::getNextPosition()
{
   Position current_position = pos;
    Position sherlock_next_position = sherlock->getCurrentPosition();

    
    Position adjacent_positions[4] = {
        Position(current_position.getRow() - 1, current_position.getCol()),
        Position(current_position.getRow(), current_position.getCol() + 1),
        Position(current_position.getRow() + 1, current_position.getCol()),
        Position(current_position.getRow(), current_position.getCol() - 1)
    };

    int min_distance = 10000; 
    Position best_position = current_position;

    for (int i = 0; i < 4; ++i) {
        if (map->isValid(adjacent_positions[i], this)) {
            int distance = criminal->manhattanDistance(adjacent_positions[i], sherlock_next_position);
            if (distance < min_distance) {
                min_distance = distance;
                best_position = adjacent_positions[i];
            }
        }
    }

    return best_position;
}

void RobotS::move()
{
    Position a;
    a= getNextPosition();
    if(a.getCol()!=-1 && a.getRow()!=-1){
        pos.setCol(a.getCol());
        pos.setRow(a.getRow());
    }
}

string RobotS::str() const
{
    char final[1000];
    int a=getDistance();
    if(a==0) sprintf(final, "Robot[pos=%s;type=S;dist=0]",pos.str().c_str());
    else sprintf(final, "Robot[pos=%s;type=S;dist=%s]",pos.str().c_str(),to_string(a).c_str());
    return final;
}

RobotType RobotS::getType() const
{
    return robot_type;
}

int RobotS::getDistance() const
{
    int a;
    a=criminal->manhattanDistance(pos,sherlock->getCurrentPosition());
    return a;
}
/*
 * CLASS: RobotSW kế thừa class Robot
 */
RobotSW::RobotSW(int index, const Position &init_pos, Map *map, Criminal *criminal, Sherlock *sherlock, Watson *watson)
    : Robot(index,init_pos,map,criminal,"RobotSW")
{
    this->index=index;
    this->pos=init_pos;
    this->map=map;
    this->robot_type=SW;
    this->criminal=criminal;
    this->sherlock=sherlock;
    this->watson=watson;
}

Position RobotSW::getNextPosition()
{
    Position current_position = pos;
    Position sherlock_next_position = sherlock->getCurrentPosition();
    Position watson_next_position = watson->getCurrentPosition();

    
    Position adjacent_positions[8] = {
        Position(pos.getRow() - 2, pos.getCol()), 
        Position(pos.getRow() - 1, pos.getCol() + 1),
        Position(pos.getRow(), pos.getCol() + 2),
        Position(pos.getRow() + 1, pos.getCol() + 1), 
        Position(pos.getRow() + 2, pos.getCol()), 
        Position(pos.getRow() + 1, pos.getCol() - 1),
        Position(pos.getRow(), pos.getCol() - 2),
        Position(pos.getRow() - 1, pos.getCol() - 1),
    };

    int min_distance = 10000; 
    Position best_position = current_position;

    for (int i = 0; i < 8; ++i) {
        if (map->isValid(adjacent_positions[i], this)) {
            int distance = criminal->manhattanDistance(adjacent_positions[i], sherlock_next_position) +
                           criminal->manhattanDistance(adjacent_positions[i], watson_next_position);
            if (distance < min_distance) {
                min_distance = distance;
                best_position = adjacent_positions[i];
            }
        }
    }

    return best_position;
}

string RobotSW::str() const
{
    char final[1000];
    int a= getDistance();
    if(a==0) sprintf(final, "Robot[pos=%s;type=SW;dist=0]",pos.str().c_str());
    else sprintf(final, "Robot[pos=%s;type=SW;dist=%s]",pos.str().c_str(),to_string(a).c_str());
    return final;
}

RobotType RobotSW::getType() const
{
    return robot_type;
}

int RobotSW::getDistance() const
{
    int a;
    a=criminal->manhattanDistance(pos,sherlock->getCurrentPosition())+criminal->manhattanDistance(pos,watson->getCurrentPosition());
    return a;
}

void RobotSW::move()
{
    Position a;
    a= getNextPosition();
    if(a.getCol()!=-1 && a.getRow()!=-1){
        pos.setCol(a.getCol());
        pos.setRow(a.getRow());
    }
}
/*
 * CLASS: ArrayMovingObject
 */
ArrayMovingObject::ArrayMovingObject(int capacity)
{
    this->capacity=capacity;
    this->count=0;
    arr_mv_objs=new MovingObject*[capacity];
    for(int i=0;i<capacity;i++){
        arr_mv_objs[i]=nullptr;
    }
}

ArrayMovingObject::~ArrayMovingObject()
{
    delete arr_mv_objs;
}

bool ArrayMovingObject::isFull() const
{
    if(count==capacity) return true;
    else return false;
}

bool ArrayMovingObject::add(MovingObject *mv_obj)
{
    if(isFull()) return false;
    arr_mv_objs[count]=mv_obj;
    count++;
    return true;
}

MovingObject *ArrayMovingObject::get(int index) const
{
    return arr_mv_objs[index];
}

int ArrayMovingObject::size() const
{
    return count;
}

string ArrayMovingObject::str() const
{
    string temp;
    for(int i=0;i<count;i++){
        if(i==count-1) temp+=arr_mv_objs[i]->str();
        else temp+=arr_mv_objs[i]->str()+";";
    }
    return "ArrayMovingObject[count="+to_string(count)+";capacity="+to_string(capacity)+";"+temp+"]";
}

bool ArrayMovingObject::checkMeet(int index) const
{
   bool result=false;
   Position pos1=arr_mv_objs[index]->getCurrentPosition();

   for (int k = 0; k < size(); k++) {
    if(k!=index){
        if(index==1){
            Position pos2= arr_mv_objs[k]->getCurrentPosition();
            if(pos1.isEqual(pos2)){
                if(k==0) return true;
                else if(k==2)   {   
                                    dynamic_cast<Sherlock*>(arr_mv_objs[1])->meet(dynamic_cast<Watson*>(arr_mv_objs[2]));
                                    result= false;
                                }
                else{
                        if(dynamic_cast<Robot*>(arr_mv_objs[k])->getType()==S){
                            dynamic_cast<Sherlock*>(arr_mv_objs[1])->meet(dynamic_cast<RobotS*>(arr_mv_objs[k]));
                            result= false;
                        }
                        else if(dynamic_cast<Robot*>(arr_mv_objs[k])->getType()==W){
                            dynamic_cast<Sherlock*>(arr_mv_objs[1])->meet(dynamic_cast<RobotW*>(arr_mv_objs[k]));
                            result= false;
                        }
                        else if(dynamic_cast<Robot*>(arr_mv_objs[k])->getType()==SW){
                            dynamic_cast<Sherlock*>(arr_mv_objs[1])->meet(dynamic_cast<RobotSW*>(arr_mv_objs[k]));
                            result= false;
                        }
                        else if(dynamic_cast<Robot*>(arr_mv_objs[k])->getType()==C){
                            bool result_1=dynamic_cast<Sherlock*>(arr_mv_objs[1])->meet(dynamic_cast<RobotC*>(arr_mv_objs[k]));
                            if(result_1==true) return true;
                            else result= false;
                        }
                    }
            }
        
        }
        //////////
        //////////
        else if(index==2){
            Position pos2=arr_mv_objs[k]->getCurrentPosition();
            if(pos1.isEqual(pos2)){
                if(k==0) return true;
                else if(k==1)   {
                                    dynamic_cast<Watson*>(arr_mv_objs[2])->meet(dynamic_cast<Sherlock*>(arr_mv_objs[1]));
                                    result= false;
                                }
                else{
                        if(dynamic_cast<Robot*>(arr_mv_objs[k])->getType()==S){
                            dynamic_cast<Watson*>(arr_mv_objs[2])->meet(dynamic_cast<RobotS*>(arr_mv_objs[k]));
                            result= false;
                        }
                        else if(dynamic_cast<Robot*>(arr_mv_objs[k])->getType()==W){
                            dynamic_cast<Watson*>(arr_mv_objs[2])->meet(dynamic_cast<RobotW*>(arr_mv_objs[k]));
                            result= false;
                        }
                        else if(dynamic_cast<Robot*>(arr_mv_objs[k])->getType()==SW){
                            dynamic_cast<Watson*>(arr_mv_objs[2])->meet(dynamic_cast<RobotSW*>(arr_mv_objs[k]));
                            result= false;
                        }
                        else if(dynamic_cast<Robot*>(arr_mv_objs[k])->getType()==C){
                            
                            dynamic_cast<Watson*>(arr_mv_objs[2])->meet(dynamic_cast<RobotC*>(arr_mv_objs[k]));
                            result= false;
                        }
                    }
            }
        }
        //////////
        //////////
        else if(index==0){
            Position pos2=arr_mv_objs[k]->getCurrentPosition();
            if(pos1.isEqual(pos2)){
                if(k==1) return true;
                else if(k==2) return true;
                else result= false;
            }
        }
        ////////////////////
        ////////////////////
        else{
            Position pos2=arr_mv_objs[k]->getCurrentPosition();
            if(pos1.isEqual(pos2)){
                if(k==0) result= false;
                else if(k==1)   {
                                    if(dynamic_cast<Robot*>(arr_mv_objs[index])->getType()==S){
                                        dynamic_cast<Sherlock*>(arr_mv_objs[1])->meet(dynamic_cast<RobotS*>(arr_mv_objs[index]));
                                        result= false;
                                    }
                                    else if(dynamic_cast<Robot*>(arr_mv_objs[index])->getType()==W){
                                        dynamic_cast<Sherlock*>(arr_mv_objs[1])->meet(dynamic_cast<RobotW*>(arr_mv_objs[index]));
                                        result=false;
                                    }
                                    else if(dynamic_cast<Robot*>(arr_mv_objs[index])->getType()==SW){
                                        dynamic_cast<Sherlock*>(arr_mv_objs[1])->meet(dynamic_cast<RobotSW*>(arr_mv_objs[index]));
                                        result= false;
                                    }
                                    else if(dynamic_cast<Robot*>(arr_mv_objs[index])->getType()==C){
                                        bool result_1=dynamic_cast<Sherlock*>(arr_mv_objs[1])->meet(dynamic_cast<RobotC*>(arr_mv_objs[index]));
                                        
                                        if(result_1==true) return true;
                                        else result= false;
                                    }
                                }
                else if(k==2)   {
                                    if(dynamic_cast<Robot*>(arr_mv_objs[index])->getType()==S){
                                        dynamic_cast<Watson*>(arr_mv_objs[2])->meet(dynamic_cast<RobotS*>(arr_mv_objs[index]));
                                        result= false;
                                    }
                                    else if(dynamic_cast<Robot*>(arr_mv_objs[index])->getType()==W){
                                        dynamic_cast<Watson*>(arr_mv_objs[2])->meet(dynamic_cast<RobotW*>(arr_mv_objs[index]));
                                        result= false;
                                    }
                                    else if(dynamic_cast<Robot*>(arr_mv_objs[index])->getType()==SW){
                                        dynamic_cast<Watson*>(arr_mv_objs[2])->meet(dynamic_cast<RobotSW*>(arr_mv_objs[index]));
                                        result= false;
                                    }
                                    else if(dynamic_cast<Robot*>(arr_mv_objs[index])->getType()==C){
                                        dynamic_cast<Watson*>(arr_mv_objs[2])->meet(dynamic_cast<RobotC*>(arr_mv_objs[index]));
                                        result= false;
                                    }
                                }
                else result= false;
            }
        }

   }

   }
    return result;
}
/*
 * CLASS: Configuration
 */
Configuration::Configuration(const string &filepath)
{
    ifstream input(filepath);
    if (!input.is_open())
    {
        return;
    }
    
    string line;
    while(getline(input,line)){
        if(line.find("MAP_NUM_ROWS=")!=string::npos)
        {
            map_num_rows=stoi(line.substr(line.find("=")+1));
        }
        else if(line.find("MAP_NUM_COLS=")!=string::npos)
        {
            map_num_cols=stoi(line.substr(line.find("=")+1));
        }
        else if(line.find("MAX_NUM_MOVING_OBJECTS=")!=string::npos)
        {
            max_num_moving_objects=stoi(line.substr(line.find("=")+1));
        }
        else if(line.find("ARRAY_WALLS")!=string::npos){
            size_t start= line.find("[");
            size_t end= line.find("]");

            string temp= line.substr(start+1,end-start-1);

            stringstream ss(temp);
            stringstream ss1(temp);
            string item;
            int count=0;

            while(getline(ss,item,';')){
                count++;
            }
            int** arr_temp;
            num_walls=count;

            arr_temp= new int*[num_walls];
            for(int i=0;i<num_walls;i++){
                arr_temp[i]=new int[2];
            }

            string item1;
            int index=0;
            while(getline(ss1,item1,';')){
                int a,b;
                sscanf(item1.c_str(), "(%d,%d)", &a, &b);
                arr_temp[index][0]=a;
                arr_temp[index][1]=b;
                index++;
            }
            arr_walls= new Position[num_walls];
            for(int i=0;i<num_walls;i++){
                arr_walls[i]=Position(arr_temp[i][0],arr_temp[i][1]);
            }
        }
        else if(line.find("ARRAY_FAKE_WALLS")!=string::npos){
            size_t start= line.find("[");
            size_t end= line.find("]");

            string temp= line.substr(start+1,end-start-1);

            stringstream ss(temp);
            stringstream ss1(temp);
            string item;
            int count=0;

            while(getline(ss,item,';')){
                count++;
            }
            
            num_fake_walls=count;

            int** arr_temp;
            arr_temp= new int*[num_fake_walls];
            for(int i=0;i<num_fake_walls;i++){
                arr_temp[i]=new int[2];
            }
            string item1;
            int index=0;
            while(getline(ss1,item1,';')){
                int a,b;
                sscanf(item1.c_str(), "(%d,%d)", &a, &b);
                arr_temp[index][0]=a;
                arr_temp[index][1]=b;
                index++;
            }
            arr_fake_walls= new Position[num_fake_walls];
            for(int i=0;i<num_fake_walls;i++){
                arr_fake_walls[i]=Position(arr_temp[i][0],arr_temp[i][1]);
            }
        }
        else if(line.find("SHERLOCK_MOVING_RULE=")!=string::npos)
        {
            sherlock_moving_rule=line.substr(line.find("=")+1);
        }
        else if(line.find("SHERLOCK_INIT_POS=")!=string::npos)
        {
            size_t start= line.find("(");
            size_t end= line.find(")");

            string temp= line.substr(start+1,end-start-1);

            int arr_temp[2];
            stringstream ss(temp);
            string item;
            getline(ss,item,',');
            arr_temp[0]=stoi(item);
            getline(ss,item,',');
            arr_temp[1]=stoi(item);
            sherlock_init_pos=Position(arr_temp[0],arr_temp[1]);
        }
        else if(line.find("SHERLOCK_INIT_HP=")!=string::npos)
        {
            sherlock_init_hp=stoi(line.substr(line.find("=")+1));
        }
        else if(line.find("SHERLOCK_INIT_EXP=")!=string::npos)
        {
            sherlock_init_exp=stoi(line.substr(line.find("=")+1));
        }
        else if(line.find("WATSON_MOVING_RULE=")!=string::npos)
        {
            watson_moving_rule=line.substr(line.find("=")+1);
        }
        else if(line.find("WATSON_INIT_POS=")!=string::npos)
        {
            size_t start= line.find("(");
            size_t end= line.find(")");

            string temp= line.substr(start+1,end-start-1);

            int arr_temp[2];
            stringstream ss(temp);
            string item;
            getline(ss,item,',');
            arr_temp[0]=stoi(item);
            getline(ss,item,',');
            arr_temp[1]=stoi(item);
            watson_init_pos=Position(arr_temp[0],arr_temp[1]);
        }
        else if(line.find("WATSON_INIT_HP=")!=string::npos)
        {
            watson_init_hp=stoi(line.substr(line.find("=")+1));
        }
        else if(line.find("WATSON_INIT_EXP=")!=string::npos)
        {
            watson_init_exp=stoi(line.substr(line.find("=")+1));
        }
        else if(line.find("CRIMINAL_INIT_POS=")!=string::npos)
        {
            size_t start= line.find("(");
            size_t end= line.find(")");

            string temp= line.substr(start+1,end-start-1);
            
            int arr_temp[2];
            stringstream ss(temp);
            string item;
            getline(ss,item,',');
            arr_temp[0]=stoi(item);
            getline(ss,item,',');
            arr_temp[1]=stoi(item);
            criminal_init_pos=Position(arr_temp[0],arr_temp[1]);
        }
        else if(line.find("NUM_STEPS=")!=string::npos)
        {
            num_steps=stoi(line.substr(line.find("=")+1));
        }
    }
}
Configuration::~Configuration()
{
    delete arr_walls;
    delete arr_fake_walls;
}
string Configuration::str() const
{   
    string arr_walls_str;
    for(int i=0;i<num_walls;i++){
        if(i==num_walls-1) arr_walls_str+=arr_walls[i].str();
        else arr_walls_str+=arr_walls[i].str()+";";
    }
    string arr_fake_walls_str;
    for(int i=0;i<num_fake_walls;i++){
        if(i==num_fake_walls-1) arr_fake_walls_str+=arr_fake_walls[i].str();
        else arr_fake_walls_str+=arr_fake_walls[i].str()+";";
    }
    return "Configuration[\nMAP_NUM_ROWS="+to_string(map_num_rows)+
    "\nMAP_NUM_COLS="+to_string(map_num_cols)+
    "\nMAX_NUM_MOVING_OBJECTS="+to_string(max_num_moving_objects)+
    "\nNUM_WALLS="+to_string(num_walls)+
    "\nARRAY_WALLS=["+arr_walls_str+"]"+
    "\nNUM_FAKE_WALLS="+to_string(num_fake_walls)+
    "\nARRAY_FAKE_WALLS=["+arr_fake_walls_str+"]"+
    "\nSHERLOCK_MOVING_RULE="+sherlock_moving_rule+
    "\nSHERLOCK_INIT_POS="+sherlock_init_pos.str()+
    "\nSHERLOCK_INIT_HP="+to_string(sherlock_init_hp)+
    "\nSHERLOCK_INIT_EXP="+to_string(sherlock_init_exp)+
    "\nWATSON_MOVING_RULE="+watson_moving_rule+
    "\nWATSON_INIT_POS="+watson_init_pos.str()+
    "\nWATSON_INIT_HP="+to_string(watson_init_hp)+
    "\nWATSON_INIT_EXP="+to_string(watson_init_exp)+
    "\nCRIMINAL_INIT_POS="+criminal_init_pos.str()+
    "\nNUM_STEPS="+to_string(num_steps)+
    "\n]";
}

// *CLASS: MagicBook

ItemType MagicBook::getType() const
{
    return ItemType::MAGIC_BOOK;
}

string MagicBook::str() const
{
    return "MagicBook";
}

bool MagicBook::canUse(Character *obj, Robot *robot)
{
    bool result=false;
    if(obj->getObjectType()==SHERLOCK ){
       
        if(((Sherlock*)obj)->getEXP()<=350 && robot==nullptr) return true;
        else return false;
    }

    if(obj->getObjectType()==WATSON ){
        
        if(((Watson*)obj)->getEXP()<=350 && robot==nullptr) return true;
        else return false;
    }   
   return result;
}

void MagicBook::use(Character *obj, Robot *robot)
{
    if(obj->getObjectType()==SHERLOCK){
        ((Sherlock*)obj)->setEXP(((Sherlock*)obj)->getEXP()+ ceil(0.25 * ((Sherlock*)obj)->getEXP()));
    }
    if(obj->getObjectType()==WATSON){
        ((Watson*)obj)->setEXP(((Watson*)obj)->getEXP()+ ceil(0.25 * ((Watson*)obj)->getEXP()));
    }
}

// *CLASS: EnergyDrink
ItemType EnergyDrink::getType() const
{
    return ItemType::ENERGY_DRINK;
}

string EnergyDrink::str() const
{
    return "EnergyDrink";
}

bool EnergyDrink::canUse(Character *obj, Robot *robot)
{   
    bool result=false;
    if(obj->getObjectType()==SHERLOCK){
        
        if(((Sherlock*)obj)->getHP()<=100 && robot==nullptr) return true;
        else return false;
    }

    if(obj->getObjectType()==WATSON){
       
        if(((Watson*)obj)->getHP()<=100 && robot==nullptr) return true;
        else return false;
    }   
    return result;
}

void EnergyDrink::use(Character *obj, Robot *robot)
{
    if(obj->getObjectType()==SHERLOCK){
        ((Sherlock*)obj)->setHP(((Sherlock*)obj)->getHP()+ ceil(0.2 * ((Sherlock*)obj)->getHP()));
    }
    if(obj->getObjectType()==WATSON){
        ((Watson*)obj)->setHP(((Watson*)obj)->getHP()+ceil(0.2 * ((Watson*)obj)->getHP()));
    }
}

// *CLASS: FirstAid
ItemType FirstAid::getType() const
{
    return ItemType::FIRST_AID;
}

string FirstAid::str() const
{
    return "FirstAid";
}

bool FirstAid::canUse(Character *obj, Robot *robot)
{   
    bool result=false;
    if(obj->getObjectType()==SHERLOCK){
        
        if((((Sherlock*)obj)->getHP()<=100 || ((Sherlock*)obj)->getEXP()<=350) && robot==nullptr) return true;
        else return false;
    }
    if(obj->getObjectType()==WATSON){
        
        if((((Watson*)obj)->getHP()<=100 || ((Watson*)obj)->getEXP()<=350) && robot==nullptr) return true;
        else return false;
    }
    return result;
}

void FirstAid::use(Character *obj, Robot *robot)
{
    if(obj->getObjectType()==SHERLOCK){
        ((Sherlock*)obj)->setHP(((Sherlock*)obj)->getHP()+ceil(0.5 * ((Sherlock*)obj)->getHP()));
    }
    if(obj->getObjectType()==WATSON){
        ((Watson*)obj)->setHP(((Watson*)obj)->getHP()+ceil(0.5 * ((Watson*)obj)->getHP()));
    }
}

// *CLASS: ExcemptionCard
ItemType ExcemptionCard::getType() const
{
    return ItemType::EXCEMPTION_CARD;
}

string ExcemptionCard::str() const
{
    return "ExcemptionCard";
}

bool ExcemptionCard::canUse(Character *obj, Robot *robot)
{
    bool result=false;
    if(obj->getObjectType()==SHERLOCK){
        
        if(((Sherlock*)obj)->getHP()%2!=0 &&  robot!=nullptr) return true;
        else return false;
    }
    if(obj->getObjectType()==WATSON) return false;
    
    return result;
}

void ExcemptionCard::use(Character *obj, Robot *robot)
{
    
}
// *CLASS: PassingCard
PassingCard::PassingCard(int i, int j)
{
    int x;
    x=ceil((i*11 + j)%4);
    
    if(x==0) this->challenge= "RobotS";
     if(x==1) this->challenge= "RobotC";
     if(x==2) this->challenge= "RobotSW";
     if(x==3) {
        //cout<<"all"<<endl;
        this->challenge= "all";}
}

ItemType PassingCard::getType() const
{
    return ItemType::PASSING_CARD;
}

string PassingCard::str() const
{
    return "PassingCard";
}

bool PassingCard::canUse(Character *obj, Robot *robot)
{
    bool result=false;
    if(obj->getObjectType()==SHERLOCK) return false;

    if(obj->getObjectType()==WATSON ){
        if(((Watson*)obj)->getHP()%2==0 && robot!=nullptr) return true;
        else return false;
    }
    return result;
}

void PassingCard::use(Character *obj, Robot *robot)
{
    //cout<<challenge<<endl;
    
    
    if(challenge!="all"){
    if(challenge=="RobotS"){
        if(robot->getType()!=S){
            ((Watson*)obj)->setEXP(((Watson*)obj)->getEXP()-50);
        }
    }
    else if(challenge=="RobotC"){
        if(robot->getType()!=C){
            ((Watson*)obj)->setEXP(((Watson*)obj)->getEXP()-50);
        }
    }
    else if(challenge=="RobotSW"){
        if(robot->getType()!=SW){
            ((Watson*)obj)->setEXP(((Watson*)obj)->getEXP()-50);
        }
    }
    }
}


// *CLASS: BaseBag
BaseItem *BaseBag::get()
{
   return nullptr;
}
BaseBag::BaseBag(int capacity)
{
    this->capacity=capacity;
    this->size=0;
}
bool BaseBag::isFull() const
{
    if(size==capacity) return true;
    else return false;
}
BaseBag::~BaseBag()
{
    for(int i=0;i<size;i++){
        if(head!=NULL){
        Node* temp=head;
        head=head->next;
        delete temp;
        }
    }
    
}

bool BaseBag::insert(BaseItem *item)
{
    if(size==capacity) return false;
    
    if(size==0) {
        Node* newNode= new Node(item);
        head=newNode;
        size++;
        return true;
    }

    Node* newNode= new Node(item);

    newNode->next=head;
    head=newNode;
    size++;
    return true;
    
}

BaseItem *BaseBag::get(ItemType itemType)
{
    if(size==0) { return nullptr;}
    Node* preNode=nullptr;
    Node* temp=head;

    if(temp->item->getType()==itemType){
        head=temp->next;
        BaseItem* foundItem=temp->item;
        delete temp;
        size--;
        return foundItem;
    }

    while(temp!= nullptr && temp->item->getType()!=itemType){
        preNode=temp;
        temp=temp->next;
    }
    
    if(temp==nullptr) return nullptr;

    BaseItem* foundItem=temp->item;
    temp->item = head->item;
    head->item = foundItem;

    Node* deleteNode=head;
    BaseItem* result= head->item;
    head=head->next;
    delete deleteNode;
    size--;
    return result;
}

string BaseBag::str() const
{
    if(size==0) return "Bag[count=0;]";
    string str;
    Node* temp=head;

    while(temp!=nullptr){
        str+=temp->item->str();
        if(temp->next!=nullptr) str+=",";
        temp=temp->next;
    }

    return "Bag[count="+to_string(size)+";"+str+"]";
}
// *CLASS: SherlockBag
SherlockBag::SherlockBag(Sherlock *character)
    : BaseBag(13)
{
    this->capacity=13;
    this->sherlock=character;
    
}

BaseItem *SherlockBag::get()
{
    if(size==0) { return nullptr;}
    Node* preNode=nullptr; 
    Node* temp=head;

    if(temp->item->canUse(sherlock,nullptr)){
        head=temp->next;
        BaseItem* foundItem=temp->item;
        delete temp;
        size--;
        return foundItem;
    }
    
    while(temp!= nullptr && !temp->item->canUse(sherlock,nullptr)){
        preNode=temp;
        temp=temp->next;
    }

    if(temp==nullptr) return nullptr;

    BaseItem* foundItem=temp->item;
    temp->item = head->item;
    head->item = foundItem;

    Node* deleteNode=head;
    BaseItem* result= head->item;
    head=head->next;
    delete deleteNode;
    size--;
    return result;

}

// *CLASS: WatsonBag
WatsonBag::WatsonBag(Watson *character)
    : BaseBag(15)
{
    this->capacity=15;
    this->watson=character;
}
BaseItem *WatsonBag::get()
{
   if(size==0) { return nullptr;}
    Node* preNode=nullptr; 
    Node* temp=head;

    if(temp->item->canUse(watson,nullptr)){
        head=temp->next;
        BaseItem* foundItem=temp->item;
        delete temp;
        size--;
        return foundItem;
    }
    
    while(temp!= nullptr && !temp->item->canUse(watson,nullptr)){
        preNode=temp;
        temp=temp->next;
    }

    if(temp==nullptr) return nullptr;

    BaseItem* foundItem=temp->item;
    temp->item = head->item;
    head->item = foundItem;

    Node* deleteNode=head;
    BaseItem* result= head->item;
    head=head->next;
    delete deleteNode;
    size--;
    return result;

}

bool BaseBag::checkItem(ItemType itemType)
{
    if(size==0) return false;
    Node* temp=head;

    while(temp!=nullptr){
        if(temp->item->getType()==itemType){
            return true;
        }
        temp=temp->next;
    }
    return false;
}

/*
CLASS: STUDYINPINK
*/
StudyPinkProgram::StudyPinkProgram(const string &config_file_path)
{
    config = new Configuration(config_file_path);
    map = new Map(config->map_num_rows, config->map_num_cols, config->num_walls, config->arr_walls, config->num_fake_walls, config->arr_fake_walls);
    arr_mv_objs = new ArrayMovingObject(config->max_num_moving_objects);
    sherlock = new Sherlock(1, config->sherlock_moving_rule, config->sherlock_init_pos, map, config->sherlock_init_hp, config->sherlock_init_exp);
    watson = new Watson(2, config->watson_moving_rule, config->watson_init_pos, map, config->watson_init_hp, config->watson_init_exp);
    criminal = new Criminal(0, config->criminal_init_pos, map, sherlock, watson);

    arr_mv_objs->add(criminal);
    arr_mv_objs->add(sherlock);
    arr_mv_objs->add(watson);
    
}

StudyPinkProgram::~StudyPinkProgram()
{
    delete map;
    delete config;
    delete arr_mv_objs;
}

bool StudyPinkProgram::isStop() const{
    if(sherlock->getHP()==0) return true;
    else if(watson->getHP()==0) return true;
    else if(criminal->getNextPosition().isEqual(sherlock->getCurrentPosition())) return true;
    else if(criminal->getNextPosition().isEqual(watson->getCurrentPosition())) return true;
    else return false;
}




void StudyPinkProgram::run(bool verbose)
{
    bool temp = false;
    /*if (!OUTPUT.is_open())
    {
        return;
    }*/
   // OUTPUT << config->str() << endl;
    temp = sherlock->getHP() == 0 || watson->getHP() == 0;
    for (int istep = 0; istep < config->num_steps && !temp; ++istep)
    {   
       printStep(istep);
       if(!isStop()) {
        if((criminal->getCount()+1)%3==0 && criminal->getCount()>0){
            MovingObject *robot = nullptr;
            robot = Robot::create(arr_mv_objs->size(), map, criminal, sherlock, watson);
            if (robot != nullptr)
            {
                    arr_mv_objs->add(robot);
            }
        }}
        int roundSize = arr_mv_objs->size();
        for (int i = 0; i < roundSize && !temp; ++i)
        {
            
            if (arr_mv_objs->get(i)->getObjectType() == ROBOT)
            {
                
                BaseItem *item = dynamic_cast<Robot *>(arr_mv_objs->get(i))->NewItem();
                
            }
           
            arr_mv_objs->get(i)->move();
            
            temp = arr_mv_objs->checkMeet(i);
            
            //printInfo(istep,i,OUTPUT);
            
        }
    }
    printResult();
    
}
