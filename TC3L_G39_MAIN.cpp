//****************************************************************************************************************************************************************
// Program: TC3L_G39_MAIN.cpp
// Course: CCP6124 OOPDS
// Lecture Class: TC3L
// Tutorial Class: T10L
// Trimester: 2410
// Member_1: 1211112310 | ALEEF NUFAYL BIN MOHD ZAINAL BADRI | 1211112310@STUDENT.MMU.EDU.MY | 011-10008858
// Member_1: 1211110669 | DARVESH SINGH | 1211110669@STUDENT.MMU.EDU.MY | 016-6978602
// Member_1: 1211109417 | OSWALD NG HAO JET | 1211109417@STUDENT.MMU.EDU.MY| 011-10730294
// Member_1: 1211110222 | WEE JIA SHEEN | 1211110222@STUDENT.MMU.EDU.MY | 016-8058871
//****************************************************************************************************************************************************************
// Task Distribution
// Member_1: GRANDPARENT CLASSES (ROBOT CLASS, BATTLEFIELD CLASS), GRANDCHILDREN CLASSES(10 ROBOT CLASSES), DATA STRUCTURE IMPLEMENTATION(VECTOR, LIST, QUEUE)
// Member 2: REPORT, PARENT CLASS, INT MAIN, DIAGRAM REPRESENTATION
// Member 4: REPORT, PARENT CLASS, INT MAIN, DEBUG AND TEST RUN
// Member 3: REPORT, PARENT CLASS, INT MAIN, LOGIC COMPUTATION
//****************************************************************************************************************************************************************

//update: -added upgrade function for every robot. -discarded all additional robots -added kills and totalkills parameter to every and any robot(s) classes. -exchanged grid to simulation

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include "Vector.h"
#include "List.h"
#include "Queue.h"
using namespace std;


class Robot
{
protected:
    int posX, posY;

    string type_;
    string alias_;
    string IDN_;
    string ID_;
    string name_;
    string fName_;
    int numOfLives_ = 3;
    int numOfKills_ = 0;
    int totalKills_ = 0;

    List<Robot*> listRobots;

    bool dead = false;

    void parseFName()
    {
        size_t underscorePos = fName_.find('_');
        if (underscorePos != string::npos)
        {
            alias_ = fName_.substr(0, 2);
            IDN_ = fName_.substr(2, underscorePos - 2);
            name_ = fName_.substr(underscorePos);
            ID_ = alias_ + IDN_;
        }
    }

public:
    Robot(int posX, int posY, string type, string alias, string IDN, string name, int kills, int totalKills, List<Robot*>& listRobots) : posX(posX), posY(posY), type_(type), alias_(alias), IDN_(IDN), name_(name), numOfKills_(kills), totalKills_(totalKills), listRobots(listRobots) { parseFName(); }

    int x() { return posX; }
    int y() { return posY; }
    getRobotPos(ostream& out) { return posX, posY; out << "(" << posX << "," << posY << ")"; }
    void setX(int x) { posX = x; }
    void setY(int y) { posY = y; }

    string getType() const { return type_; }
    string getFullName() const { return alias_ + IDN_ + name_; }
    string getFN() const { return fName_; }
    string getName() const { return name_; }
    string getIDN() const { return IDN_; }
    string getID() const { return alias_ + IDN_; }
    string getAlias() const { return alias_; }
    string setFullName(string Alias, string IDN, string Name)
    {
        alias_ = Alias;
        IDN_ = IDN;
        name_ = Name;
        return alias_ + IDN_ + name_;
    }

    int getLives() { return numOfLives_; }
    int getKills() { return numOfKills_; }
    void incKills() { numOfKills_++; totalKills_++; }
    int getTotalKills() { return totalKills_; }
    void resetKills() { numOfKills_ = 0; }

    void decLife() { numOfLives_--; }
    void nowDead() { dead = true; }
    void nowLive() { dead = false; }
    bool isDead() const { return dead;}
    bool isAlive()
    {
        if (numOfLives_ > 0)
            return true;
        else
            return false;
    }

    void getDetail(ostream& out)
    {
        out << "\n" << getFullName() << " | ";
        out << "Live(s): " << getLives() << " | ";
        out << "Kill(s): " << getKills() << " | ";
        out << "Total Kill(s): " << getTotalKills();
    }

    friend ostream& operator << (ostream &out, const Robot& r)
    {
        out << r.getFullName();
        return out;
    }

    virtual void actions(Queue<Robot*>& queueWaitingRobots, Queue<Robot*>& queueDestroyedRobots, List<Robot*> listRobots, ostream& out) = 0;
    virtual Robot* upgrade(List<Robot*>& listRobots, ostream& out) = 0;

    string getQ(Queue<Robot*>& queueWaitingRobots, Queue<Robot*>& queueDestroyedRobots, ostream& out)
    {
        out << "\nQueue Waiting Robots: ";
        queueWaitingRobots.printQueue(out);

        out << "\nQueue Destroyed Robots: ";
        queueDestroyedRobots.printQueue(out);

        out << endl;
        return "";
    }
};


class Battlefield
{
protected:
    List<Robot*> listRobots;

private:
    int rows;
    int cols;
    string** simulation;
    string** box;
    string** casket;
    string** shelf;
    Robot*** robots;
    Robot*** outRobots;
    Robot*** deadRobots;
    Robot*** oldRobots;

public:
    Battlefield(int rows, int cols) : rows(rows), cols(cols)
    {
        simulation = new string*[rows];
        robots = new Robot**[rows];
        for (int i = 0; i < rows; ++i)
        {
            simulation[i] = new string[cols];
            robots[i] = new Robot*[cols];
            for (int j = 0; j < cols; ++j)
            {
                simulation[i][j] = "    ";
                robots[i][j] = nullptr;
            }
        }
        const int bs = 100;
        box = new string*[bs];
        outRobots = new Robot**[bs];
        for (int i = 0; i < bs; ++i)
        {
            box[i] = new string[bs];
            outRobots[i] = new Robot*[bs];
            for (int j = 0; j < bs; ++j)
            {
                box[i][j] = "    ";
                outRobots[i][j] = nullptr;
            }
        }
        const int cs = 100;
        casket = new string*[cs];
        deadRobots = new Robot**[cs];
        for (int i = 0; i < cs; ++i)
        {
            casket[i] = new string[cs];
            deadRobots[i] = new Robot*[cs];
            for (int j = 0; j < cs; ++j)
            {
                casket[i][j] = "    ";
                deadRobots[i][j] = nullptr;
            }
        }
        const int ss = 100;
        shelf = new string*[ss];
        oldRobots = new Robot**[ss];
        for (int i = 0; i < ss; ++i)
        {
            shelf[i] = new string[ss];
            oldRobots[i] = new Robot*[ss];
            for (int j = 0; j < ss; ++j)
            {
                shelf[i][j] = "    ";
                oldRobots[i][j] = nullptr;
            }
        }
    }

    int getRows() { return rows; }
    int getCols() { return cols; }

    bool isEmpty(int x, int y) { return simulation[y][x] == "    "; }
    bool isInside(int x, int y) { return x >= 0 && x < cols && y >= 0 && y < rows; }

    void moveRobot(int oldX, int oldY, int newX, int newY)
    {
        if (newX == -1 && newY == -1)
        {
            for (int i = 0; i < 100; ++i)
            {
                for (int j = 0; j < 100; ++j)
                {
                    if (outRobots[i][j] == nullptr)
                    {
                        box[i][j] = simulation[oldY][oldX];
                        simulation[oldY][oldX] = "    ";
                        outRobots[i][j] = robots[oldY][oldX];
                        robots[oldY][oldX] = nullptr;
                    }
                }
            }
        }
        else if (oldX == -1 &&  oldY == -1)
        {
            for (int i = 0; i < 100; ++i)
            {
                for (int j = 0; j < 100; ++j)
                {
                    if (outRobots[i][j] != nullptr)
                    {
                        simulation[newY][newX] = box[i][j];
                        robots[newY][newX] = outRobots[i][j];
                        box[i][j] = "    ";
                        outRobots[i][j] = nullptr;
                        return;
                    }
                }
            }
        }
        else if (newX == -2 && newY == -2)
        {
            for (int i = 0; i < 100; ++i)
            {
                for (int j = 0; j < 100; ++j)
                {
                    if (deadRobots[i][j] == nullptr)
                    {
                        casket[i][j] = simulation[oldY][oldX];
                        simulation[oldY][oldX] = "    ";
                        deadRobots[i][j] = robots[oldY][oldX];
                        robots[oldY][oldX] = nullptr;
                    }
                }
            }
        }
        else if (newX == -3 && newY == -3)
        {
            for (int i = 0; i < 100; ++i)
            {
                for (int j = 0; j < 100; ++j)
                {
                    if (oldRobots[i][j] == nullptr)
                    {
                        shelf[i][j] = simulation[oldY][oldX];
                        simulation[oldY][oldX] = "    ";
                        oldRobots[i][j] = robots[oldY][oldX];
                        robots[oldY][oldX] = nullptr;
                    }
                }
            }
        }
        else
        {
        simulation[newY][newX] = simulation[oldY][oldX];
        simulation[oldY][oldX] = "    ";
        robots[newY][newX] = robots[oldY][oldX];
        robots[oldY][oldX] = nullptr;
        }
    }

    void placeRobot(int x, int y, Robot* robot, ostream& out)
    {
        if (isInside(x, y) && isEmpty(x, y))
        {
        simulation[y][x] = robot->getID();
        robots[y][x] = robot;
        }
        else
        {
            out << "ERROR.placeRobot\n";
        }
    }

    void display(ostream& out) const
    {
        out << "Display Battlefield\n\n";

        out << "      ";
        for (int j = 0; j < cols; ++j)
        {
            out << setw(3) << setfill(' ') << j << "  ";
        }
        out << "\n    +";
        for (int j = 0; j < cols; ++j)
        {
            out << "----+";
        }
        out << "\n";

        for (int i = 0; i < rows; ++i)
        {
            out << setw(2) << setfill(' ') << i << "  |";
            for (int j = 0; j < cols; ++j)
            {
                out << setw(4) << setfill(' ') << simulation[i][j] << "|";
            }
            out << "\n    +";
            for (int j = 0; j < cols; ++j)
            {
                out << "----+";
            }
            out << "\n";
        }
    }

    Robot* getRobotAt(int x, int y)
    {
        return robots[y][x];
    }
};


class SeeingRobot : virtual public Robot
{
protected:
    Battlefield* battlefield;

public:
    SeeingRobot(int posX, int posY, string type, string alias, string IDN, string name, int kills, int totalKills, List<Robot*>& listRobots, Battlefield* battlefield)
        : Robot(posX, posY, type, alias, IDN, name, kills, totalKills, listRobots), battlefield(battlefield) {}

    virtual void actionLook(ostream& out) = 0;
};


class MovingRobot : virtual public Robot
{
protected:
    Battlefield* battlefield;

public:
    MovingRobot(int posX, int posY, string type, string alias, string IDN, string name, int kills, int totalKills, List<Robot*>& listRobots, Battlefield* battlefield)
        : Robot(posX, posY, type, alias, IDN, name, kills, totalKills, listRobots), battlefield(battlefield) {}

    virtual void actionMove(ostream& out) = 0;
};


class ShootingRobot : virtual public Robot
{
protected:
    Battlefield* battlefield;

public:
    ShootingRobot(int posX, int posY, string type, string alias, string IDN, string name, int kills, int totalKills, List<Robot*>& listRobots, Battlefield* battlefield)
        : Robot(posX, posY, type, alias, IDN, name, kills, totalKills, listRobots), battlefield(battlefield) {}

    virtual void actionFire(Queue<Robot*>& queueWaitingRobots, Queue<Robot*>& queueDestroyedRobots, List<Robot*> listRobots, ostream& out) = 0;
};


class SteppingRobot : virtual public Robot
{
protected:
    Battlefield* battlefield;

public:
    SteppingRobot(int posX, int posY, string type, string alias, string IDN, string name, int kills, int totalKills, List<Robot*>& listRobots, Battlefield* battlefield)
        : Robot(posX, posY, type, alias, IDN, name, kills, totalKills, listRobots), battlefield(battlefield) {}

    virtual void actionStep(Queue<Robot*>& queueWaitingRobots, Queue<Robot*>& queueDestroyedRobots, List<Robot*> listRobots, ostream& out) = 0;
};


class UltimateRobot : virtual public SeeingRobot, virtual public MovingRobot, virtual public ShootingRobot, virtual public SteppingRobot
{
protected:
    Battlefield* battlefield;

public:
    UltimateRobot(int x, int y, string type, string alias, string IDN, string name, int kills, int totalKills, List<Robot*>& listRobots, Battlefield* battlefield)
        : Robot(x, y, type,alias, IDN, name, kills, totalKills, listRobots), battlefield(battlefield),
          SeeingRobot(x, y, type, alias, IDN, name, kills, totalKills, listRobots, battlefield),
          MovingRobot(x, y, type, alias, IDN, name, kills, totalKills, listRobots, battlefield),
          ShootingRobot(x, y, type, alias, IDN, name, kills, totalKills, listRobots, battlefield),
          SteppingRobot(x, y, type, alias, IDN, name, kills, totalKills, listRobots, battlefield) {robotAutoIncInt_++;}

    static int robotAutoIncInt_;

    Robot* upgrade(List<Robot*>& listRobots, ostream& out) override {return nullptr;}

    void actionLook(ostream& out) override
    {
        out << "\nSeeingRobot actionLook\n\n";
        out << "Surrounding positions:\n";

        int minX = max(0, x() - 1);
        int maxX = min(battlefield->getCols() - 1, x() + 1);
        int minY = max(0, y() - 1);
        int maxY = min(battlefield->getRows() - 1, y() + 1);

        for (int newY = minY; newY <= maxY; ++newY)
        {
            for (int newX = minX; newX <= maxX; ++newX)
            {
                if (newX == x() && newY == y()) continue;

                out << "(" << newX << ", " << newY << "): ";

                if (battlefield->isInside(newX, newY))
                {
                    if (battlefield->isEmpty(newX, newY))
                    {
                        out << "Empty";
                    }
                    else
                    {
                        Robot* robot = battlefield->getRobotAt(newX, newY);
                        if (robot)
                        {
                            out << "Occupied by " << robot->getID();
                        }
                    }
                }
                else
                {
                    out << "Outside of battlefield.";
                }
                out << "\n";
            }
        }
    }

    void actionMove(ostream& out) override
    {
        out << "\nMovingRobot actionMove\n\n";

        Vector<pair<int, int>> possibleMoves;

        int minX = max(0, x() - 1);
        int maxX = min(battlefield->getCols() - 1, x() + 1);
        int minY = max(0, y() - 1);
        int maxY = min(battlefield->getRows() - 1, y() + 1);

        for (int newX = minX; newX <= maxX; ++newX)
        {
            for (int newY = minY; newY <= maxY; ++newY)
            {
                if (newX == x() && newY == y()) continue;

                if (battlefield->isInside(newX, newY) && battlefield->isEmpty(newX, newY))
                {
                    possibleMoves.push_back(make_pair(newX, newY));
                }
            }
        }

        if (!possibleMoves.empty())
        {
            int randomIndex = rand() % possibleMoves.size();
            int newX = possibleMoves[randomIndex].first;
            int newY = possibleMoves[randomIndex].second;

            battlefield->moveRobot(x(), y(), newX, newY);
            setX(newX);
            setY(newY);

            out << "Robot " << getID() << " moved to (" << newX << ", " << newY << ")\n";
        }
        else
        {
            out << "No available move for " << getID() << endl;
        }
    }

    void actionFire(Queue<Robot*>& queueWaitingRobots, Queue<Robot*>& queueDestroyedRobots, List<Robot*> listRobots, ostream& out) override
    {
        out << "\nShootingRobot actionFire\n";

        for (int i = 0; i < 3; i++)
        {
            int targetX, targetY;
            int randX = rand() % battlefield->getCols();
            int randY = rand() % battlefield->getRows();

            targetX = randX;
            targetY = randY;

            if (targetX == x())
            { targetX = rand() % battlefield->getCols(); }

            if (targetY == y())
            { targetY = rand() % battlefield->getRows(); }

            out << endl << getID() << " fired at (" << targetX << ", " << targetY << ")\n";

            Robot* targetRobot = battlefield->getRobotAt(targetX, targetY);
            if (targetRobot)
            {
                out << getID() << " hit " << targetRobot->getID() << " at (" << targetX << ", " << targetY << ")\n";
                this->incKills();
                targetRobot->decLife();
                targetRobot->nowDead();

                if (targetRobot->isAlive())
                {
                    queueWaitingRobots.enqueue(targetRobot);
                    Robot* r = queueWaitingRobots.front();
                    battlefield->moveRobot(targetX, targetY, -1, -1);
                    targetRobot->setX(-1);
                    targetRobot->setY(-1);
                    out << "\n!----------------------- Robot Hit -----------------------!\n";
                    targetRobot->getDetail(out);
                    out << endl << "\n!---------------------------------------------------------!\n";
                }
                else
                {
                    queueDestroyedRobots.enqueue(targetRobot);
                    Robot* r = queueDestroyedRobots.front();
                    battlefield->moveRobot(targetX, targetY, -2, -2);
                    targetRobot->setX(-2);
                    targetRobot->setY(-2);
                    out << "\n!----------------------- Robot Died -----------------------!\n\n";
                    out << targetRobot->getID() << " ran out of lives\n";
                    targetRobot->getDetail(out);
                    out << endl << "\n!---------------------------------------------------------!\n";
                }
            }
            else
            {
                out << "No robot hit\n";
            }
        }
    }

    void actionStep(Queue<Robot*>& queueWaitingRobots, Queue<Robot*>& queueDestroyedRobots, List<Robot*> listRobots, ostream& out) override
    {
        out << "\nSteppingRobot actionStep\n\n";

        Vector<pair<int, int>> possibleMovesWithRobots;
        Vector<pair<int, int>> possibleMovesWithoutRobots;

        for (int dx = -1; dx <= 1; ++dx)
        {
            for (int dy = -1; dy <= 1; ++dy)
            {
                if (dx == 0 && dy == 0) continue;

                int newX = x() + dx;
                int newY = y() + dy;

                if (battlefield->isInside(newX, newY))
                {
                    if (!battlefield->isEmpty(newX, newY))
                    {
                        possibleMovesWithRobots.push_back(make_pair(newX, newY));
                    }
                    else
                    {
                        possibleMovesWithoutRobots.push_back(make_pair(newX, newY));
                    }
                }
            }
        }

        if (!possibleMovesWithRobots.empty())
        {
            int randomIndex = rand() % possibleMovesWithRobots.size();
            int steppedX = possibleMovesWithRobots[randomIndex].first;
            int steppedY = possibleMovesWithRobots[randomIndex].second;

            Robot* targetRobot = battlefield->getRobotAt(steppedX, steppedY);
            if (targetRobot)
            {
                out << getID() << " stepped on " << targetRobot->getID() << " at (" << steppedX << ", " << steppedY << ")\n";
                this->incKills();
                targetRobot->decLife();
                targetRobot->nowDead();

                if (targetRobot->isAlive())
                {
                    queueWaitingRobots.enqueue(targetRobot);
                    Robot* r = queueWaitingRobots.front();
                    battlefield->moveRobot(steppedX, steppedY, -1, -1);
                    targetRobot->setX(-1);
                    targetRobot->setY(-1);
                    out << "\n!----------------------- Robot Hit -----------------------!\n";
                    targetRobot->getDetail(out);
                    out << endl << "\n!---------------------------------------------------------!\n";
                }
                else
                {
                    queueDestroyedRobots.enqueue(targetRobot);
                    Robot* r = queueDestroyedRobots.front();
                    battlefield->moveRobot(steppedX, steppedY, -2, -2);
                    targetRobot->setX(-2);
                    targetRobot->setY(-2);
                    out << "\n!----------------------- Robot Died -----------------------!\n\n";
                    out << targetRobot->getID() << " ran out of lives\n";
                    targetRobot->getDetail(out);
                    out << endl << "\n!---------------------------------------------------------!\n";

                }
            }
            battlefield->moveRobot(x(), y(), steppedX, steppedY);
            setX(steppedX);
            setY(steppedY);
        }
        else if (!possibleMovesWithoutRobots.empty())
        {
            int randomIndex = rand() % possibleMovesWithoutRobots.size();
            int newX = possibleMovesWithoutRobots[randomIndex].first;
            int newY = possibleMovesWithoutRobots[randomIndex].second;

            battlefield->moveRobot(x(), y(), newX, newY);
            setX(newX);
            setY(newY);

            out << getID() << " stepped to (" << newX << ", " << newY << ")\n";
        }
    }

    void actions(Queue<Robot*>& queueWaitingRobots, Queue<Robot*>& queueDestroyedRobots, List<Robot*> listRobots, ostream& out) override
    {
        actionLook(out);
        actionMove(out);
        actionFire(queueWaitingRobots, queueDestroyedRobots, listRobots, out);
        actionStep(queueWaitingRobots, queueDestroyedRobots, listRobots, out);
        out << endl << "Robot Update:";
        getDetail(out);
        out << endl;
    }
};


class RoboTank : virtual public ShootingRobot
{
protected:
    Battlefield* battlefield;

public:
    RoboTank(int x, int y, string type, string alias, string IDN, string name, int kills, int totalKills, List<Robot*>& listRobots, Battlefield* battlefield)
        : Robot(x, y, type, alias, IDN, name, kills, totalKills, listRobots), battlefield(battlefield),
          ShootingRobot(x, y, type, alias, IDN, name, kills, totalKills, listRobots, battlefield) {robotAutoIncInt_++;}

    static int robotAutoIncInt_;

    Robot* upgrade(List<Robot*>& listRobots,  ostream& out) override
    {
        RoboTank::robotAutoIncInt_--;
        out << endl << getFullName() << " HAS BEEN UPGRADED TO (UltimateRobot) with ID: UR";

        int tempIDN = UltimateRobot::robotAutoIncInt_ + 1;
        string newIDN;

        if(tempIDN<10)
            newIDN = "0" + to_string(tempIDN);
        else
            newIDN = to_string(tempIDN);

        out << newIDN << getName() << endl;
        this->setFullName("UR", newIDN, getName());

        Robot* upgradedRobot = new UltimateRobot(x(), y(), "UltimateRobot", "UR", newIDN, getName(), getKills(), getTotalKills(), listRobots, battlefield);
        return upgradedRobot;
    }

    void actionFire(Queue<Robot*>& queueWaitingRobots, Queue<Robot*>& queueDestroyedRobots, List<Robot*> listRobots, ostream& out) override
    {
        out << "\nShootingRobot actionFire\n";

        const int shots = 1;
        for (int i = 0; i < shots; ++i)
        {
            int targetX = rand() % battlefield->getCols();
            int targetY = rand() % battlefield->getRows();

            if (targetX == x())
            { targetX = rand() % battlefield->getCols(); }

            if (targetY == y())
            { targetY = rand() % battlefield->getRows(); }

            out << endl << getID() << " fired at (" << targetX << ", " << targetY << ")\n";

            Robot* targetRobot = battlefield->getRobotAt(targetX, targetY);
            if (targetRobot)
            {
                out << getID() << " hit " << targetRobot->getID() << " at (" << targetX << ", " << targetY << ")\n";
                this->incKills();
                targetRobot->decLife();
                targetRobot->nowDead();

                if (targetRobot->isAlive())
                {
                    queueWaitingRobots.enqueue(targetRobot);
                    Robot* r = queueWaitingRobots.front();
                    battlefield->moveRobot(targetX, targetY, -1, -1);
                    targetRobot->setX(-1);
                    targetRobot->setY(-1);
                    out << "\n!----------------------- Robot Hit -----------------------!\n";
                    targetRobot->getDetail(out);
                    out << endl << "\n!---------------------------------------------------------!\n";
                }
                else
                {
                    queueDestroyedRobots.enqueue(targetRobot);
                    Robot* r = queueDestroyedRobots.front();
                    battlefield->moveRobot(targetX, targetY, -2, -2);
                    targetRobot->setX(-2);
                    targetRobot->setY(-2);
                    out << "\n!----------------------- Robot Died -----------------------!\n\n";
                    out << targetRobot->getID() << " ran out of lives\n";
                    targetRobot->getDetail(out);
                    out << endl << "\n!---------------------------------------------------------!\n";

                }
            }
            else
            {
                out << "No robot hit\n";
            }
        }
    }

    void actions(Queue<Robot*>& queueWaitingRobots, Queue<Robot*>& queueDestroyedRobots, List<Robot*> listRobots, ostream& out) override
    {
        actionFire(queueWaitingRobots, queueDestroyedRobots, listRobots, out);
        out << endl << "Robot Update:";
        getDetail(out);
        out << endl;
    }
};


class Madbot : virtual public ShootingRobot
{
protected:
    Battlefield* battlefield;

public:
    Madbot(int x, int y, string type, string alias, string IDN, string name, int kills, int totalKills, List<Robot*>& listRobots, Battlefield* battlefield)
        : Robot(x, y, type, alias, IDN, name, kills, totalKills, listRobots), battlefield(battlefield),
          ShootingRobot(x, y, type, alias, IDN, name, kills, totalKills, listRobots, battlefield) {robotAutoIncInt_++;}

    static int robotAutoIncInt_;

    Robot* upgrade(List<Robot*>& listRobots, ostream& out) override
    {
        Madbot::robotAutoIncInt_--;
        out << endl << getFullName() << " HAS BEEN UPGRADED TO (RoboTank) with ID: RT";

        int tempIDN = RoboTank::robotAutoIncInt_ + 1;
        string newIDN;

        if(tempIDN<10)
            newIDN = "0" + to_string(tempIDN);
        else
            newIDN = to_string(tempIDN);

        out << newIDN << getName() << endl;
        this->setFullName("RT", newIDN, getName());

        Robot* upgradedRobot = new RoboTank(x(), y(), "RoboTank", "RT", newIDN, getName(), getKills(), getTotalKills(), listRobots, battlefield);
        return upgradedRobot;
    }

    void actionFire(Queue<Robot*>& queueWaitingRobots, Queue<Robot*>& queueDestroyedRobots, List<Robot*> listRobots, ostream& out) override
    {
        out << "\nShootingRobot actionFire\n";

        int dx[] = { -1, -1, -1, 0, 1, 1, 1, 0 };
        int dy[] = { -1, 0, 1, 1, 1, 0, -1, -1 };

        int direction = rand() % 8;

        int targetX = x() + dx[direction];
        int targetY = y() + dy[direction];

        if (targetX < 0)
            targetX = - targetX;
        if (targetY < 0)
            targetY = - targetY;

        if (targetX > battlefield->getCols() - 1)
            targetX = targetX - battlefield->getCols();
        if (targetY > battlefield->getRows() - 1)
            targetY = targetY - battlefield->getRows();

        out << endl << getID() << " fired at (" << targetX << ", " << targetY << ")\n";

        Robot* targetRobot = battlefield->getRobotAt(targetX, targetY);
        if (targetRobot)
        {
            out << getID() << " hit " << targetRobot->getID() << " at (" << targetX << ", " << targetY << ")\n";
            this->incKills();
            targetRobot->decLife();
            targetRobot->nowDead();

            if (targetRobot->isAlive())
            {
                queueWaitingRobots.enqueue(targetRobot);
                Robot* r = queueWaitingRobots.front();
                battlefield->moveRobot(targetX, targetY, -1, -1);
                targetRobot->setX(-1);
                targetRobot->setY(-1);
                out << "\n!----------------------- Robot Hit -----------------------!\n";
                targetRobot->getDetail(out);
                out << endl << "\n!---------------------------------------------------------!\n";
            }
            else
            {
                queueDestroyedRobots.enqueue(targetRobot);
                Robot* r = queueDestroyedRobots.front();
                battlefield->moveRobot(targetX, targetY, -2, -2);
                targetRobot->setX(-2);
                targetRobot->setY(-2);
                out << "\n!----------------------- Robot Died -----------------------!\n\n";
                out << targetRobot->getID() << " ran out of lives\n";
                targetRobot->getDetail(out);
                out << endl << "\n!---------------------------------------------------------!\n";

            }
        }
        else
        {
            out << "No robot hit\n";
        }
    }

    void actions(Queue<Robot*>& queueWaitingRobots, Queue<Robot*>& queueDestroyedRobots, List<Robot*> listRobots, ostream& out) override
    {
        actionFire(queueWaitingRobots, queueDestroyedRobots, listRobots, out);
        out << endl << "Robot Update:";
        getDetail(out);
        out << endl;
    }
};


class BlueThunder : virtual public ShootingRobot
{
protected:
    Battlefield* battlefield;

private:
    int currentDirection_;

public:
    BlueThunder(int x, int y, string type, string alias, string IDN, string name, int kills, int totalKills, List<Robot*>& listRobots, Battlefield* battlefield)
        : Robot(x, y, type, alias, IDN, name, kills, totalKills, listRobots), battlefield(battlefield),
          ShootingRobot(x, y, type, alias, IDN, name, kills, totalKills, listRobots, battlefield),
          currentDirection_(0) {robotAutoIncInt_++;}

    static int robotAutoIncInt_;

    Robot* upgrade(List<Robot*>& listRobots,  ostream& out) override
    {
        BlueThunder::robotAutoIncInt_--;
        out << endl << getFullName() << " HAS BEEN UPGRADED TO (Madbot) with ID: MB";

        int tempIDN = Madbot::robotAutoIncInt_ + 1;
        string newIDN;

        if(tempIDN<10)
            newIDN = "0" + to_string(tempIDN);
        else
            newIDN = to_string(tempIDN);

        out << newIDN << getName() << endl;
        this->setFullName("MB", newIDN, getName());

        Robot* upgradedRobot = new Madbot(x(), y(), "Madbot", "MB", newIDN, getName(), getKills(), getTotalKills(), listRobots, battlefield);
        return upgradedRobot;
    }

    void actionFire(Queue<Robot*>& queueWaitingRobots, Queue<Robot*>& queueDestroyedRobots, List<Robot*> listRobots, ostream& out) override
    {
        out << "\nShootingRobot actionFire\n";

        int dx[] = { 0, 1, 1, 1, 0, -1, -1, -1 };
        int dy[] = { -1, -1, 0, 1, 1, 1, 0, -1 };

        int targetX = x() + dx[currentDirection_];
        int targetY = y() + dy[currentDirection_];

        currentDirection_ = (currentDirection_ + 1) % 8;
        int initialDirection = currentDirection_;

        while (targetX < 0 || targetX >= battlefield->getCols() || targetY < 0 || targetY >= battlefield->getRows() || (targetX == x() && targetY == y()))
        {
            currentDirection_ = (currentDirection_ + 1) % 8;
            targetX = x() + dx[currentDirection_];
            targetY = y() + dy[currentDirection_];

            if (currentDirection_ == initialDirection)
                break;
        }

        out << endl << getID() << " fired at (" << targetX << ", " << targetY << ")\n";

        Robot* targetRobot = battlefield->getRobotAt(targetX, targetY);
        if (targetRobot)
        {
            out << getID() << " hit " << targetRobot->getID() << " at (" << targetX << ", " << targetY << ")\n";
            this->incKills();
            targetRobot->decLife();
            targetRobot->nowDead();

            if (targetRobot->isAlive())
            {
                queueWaitingRobots.enqueue(targetRobot);
                Robot* r = queueWaitingRobots.front();
                battlefield->moveRobot(targetX, targetY, -1, -1);
                targetRobot->setX(-1);
                targetRobot->setY(-1);
                out << "\n!----------------------- Robot Hit -----------------------!\n";
                targetRobot->getDetail(out);
                out << endl << "\n!---------------------------------------------------------!\n";
            }
            else
            {
                queueDestroyedRobots.enqueue(targetRobot);
                Robot* r = queueDestroyedRobots.front();
                battlefield->moveRobot(targetX, targetY, -2, -2);
                targetRobot->setX(-2);
                targetRobot->setY(-2);
                out << "\n!----------------------- Robot Died -----------------------!\n\n";
                out << targetRobot->getID() << " ran out of lives\n";
                targetRobot->getDetail(out);
                out << endl << "\n!---------------------------------------------------------!\n";

            }
        }
        else
        {
            out << "No robot hit\n";
        }
    }

    void actions(Queue<Robot*>& queueWaitingRobots, Queue<Robot*>& queueDestroyedRobots, List<Robot*> listRobots, ostream& out) override
    {
        actionFire(queueWaitingRobots, queueDestroyedRobots, listRobots, out);
        out << endl << "Robot Update:";
        getDetail(out);
        out << endl;
    }
};


class TerminatorRoboCop : virtual public SeeingRobot, virtual public ShootingRobot, virtual SteppingRobot
{
protected:
    Battlefield* battlefield;

public:
    TerminatorRoboCop(int x, int y, string type, string alias, string IDN, string name, int kills, int totalKills, List<Robot*>& listRobots, Battlefield* battlefield)
        : Robot(x, y, type, alias, IDN, name, kills, totalKills, listRobots), battlefield(battlefield),
          SeeingRobot(x, y, type, alias, IDN, name, kills, totalKills, listRobots, battlefield),
          ShootingRobot(x, y, type, alias, IDN, name, kills, totalKills, listRobots, battlefield),
          SteppingRobot(x, y, type, alias, IDN, name, kills, totalKills, listRobots, battlefield) {robotAutoIncInt_++;}

    static int robotAutoIncInt_;

    Robot* upgrade(List<Robot*>& listRobots, ostream& out) override
    {
        TerminatorRoboCop::robotAutoIncInt_--;
        out << endl << getFullName() << " HAS BEEN UPGRADED TO (UltimateRobot) with ID: UR";

        int tempIDN = UltimateRobot::robotAutoIncInt_ + 1;
        string newIDN;

        if(tempIDN<10)
            newIDN = "0" + to_string(tempIDN);
        else
            newIDN = to_string(tempIDN);

        out << newIDN << getName() << endl;
        this->setFullName("UR", newIDN, getName());

        Robot* upgradedRobot = new UltimateRobot(x(), y(), "UltimateRobot", "UR", newIDN, getName(), getKills(), getTotalKills(), listRobots, battlefield);
        return upgradedRobot;
    }

    void actionLook(ostream& out) override
    {
        out << "\nSeeingRobot actionLook\n\n";
        out << "Surrounding positions:\n";

        int minX = max(0, x() - 1);
        int maxX = min(battlefield->getCols() - 1, x() + 1);
        int minY = max(0, y() - 1);
        int maxY = min(battlefield->getRows() - 1, y() + 1);

        for (int newY = minY; newY <= maxY; ++newY)
        {
            for (int newX = minX; newX <= maxX; ++newX)
            {
                if (newX == x() && newY == y()) continue;

                out << "(" << newX << ", " << newY << "): ";

                if (battlefield->isInside(newX, newY))
                {
                    if (battlefield->isEmpty(newX, newY))
                    {
                        out << "Empty";
                    }
                    else
                    {
                        Robot* robot = battlefield->getRobotAt(newX, newY);
                        if (robot)
                        {
                            out << "Occupied by " << robot->getID();
                        }
                    }
                }
                else
                {
                    out << "Outside of battlefield.";
                }
                out << "\n";
            }
        }
    }

    void actionFire(Queue<Robot*>& queueWaitingRobots, Queue<Robot*>& queueDestroyedRobots, List<Robot*> listRobots, ostream& out) override
    {
        out << "\nShootingRobot actionFire\n";

        for (int i = 0; i < 3; i++)
        {
            const int maxDistance = 10;
            int targetX, targetY;
            int negX = rand()%2;
            int negY = rand()%2;
            int randX = rand()%11;
            int randY = rand()%(11-randX);

            if (negX == 0 && negY == 0)
            {
                targetX = x() + randX;
                targetY = y() + randY;
            }
            else if (negX == 1 && negY == 0)
            {
                targetX = x() - randX;
                targetY = y() + randY;
            }
            else if (negX == 0 && negY == 1)
            {
                targetX = x() + randX;
                targetY = y() - randY;
            }
            else
            {
                targetX = x() - randX;
                targetY = y() - randY;
            }

            if (targetX < 0)
                targetX = - targetX;
            if (targetY < 0)
                targetY = - targetY;

            if (targetX > battlefield->getCols()-2)
                targetX = randX - 0.5*x();
            if (targetY > battlefield->getRows()-2)
                targetY = randY - 0.5*y();

            if (targetX < 0)
                targetX = - targetX;
            if (targetY < 0)
                targetY = - targetY;

            if (targetX == x() && targetY == y())
                targetX++;

            out << endl << getID() << " fired at (" << targetX << ", " << targetY << ")\n";

            Robot* targetRobot = battlefield->getRobotAt(targetX, targetY);
            if (targetRobot)
            {
                out << getID() << " hit " << targetRobot->getID() << " at (" << targetX << ", " << targetY << ")\n";
                this->incKills();
                targetRobot->decLife();
                targetRobot->nowDead();

                if (targetRobot->isAlive())
                {
                    queueWaitingRobots.enqueue(targetRobot);
                    Robot* r = queueWaitingRobots.front();
                    battlefield->moveRobot(targetX, targetY, -1, -1);
                    targetRobot->setX(-1);
                    targetRobot->setY(-1);
                    out << "\n!----------------------- Robot Hit -----------------------!\n";
                    targetRobot->getDetail(out);
                    out << endl << "\n!---------------------------------------------------------!\n";
                }
                else
                {
                    queueDestroyedRobots.enqueue(targetRobot);
                    Robot* r = queueDestroyedRobots.front();
                    battlefield->moveRobot(targetX, targetY, -2, -2);
                    targetRobot->setX(-2);
                    targetRobot->setY(-2);
                    out << "\n!----------------------- Robot Died -----------------------!\n\n";
                    out << targetRobot->getID() << " ran out of lives\n";
                    targetRobot->getDetail(out);
                    out << endl << "\n!---------------------------------------------------------!\n";

                }
            }
            else
            {
                out << "No robot hit\n";
            }
        }
    }

    void actionStep(Queue<Robot*>& queueWaitingRobots, Queue<Robot*>& queueDestroyedRobots, List<Robot*> listRobots, ostream& out) override
    {
        out << "\nSteppingRobot actionStep\n\n";

        Vector<pair<int, int>> possibleMovesWithRobots;
        Vector<pair<int, int>> possibleMovesWithoutRobots;

        for (int dx = -1; dx <= 1; ++dx)
        {
            for (int dy = -1; dy <= 1; ++dy)
            {
                if (dx == 0 && dy == 0) continue;

                int newX = x() + dx;
                int newY = y() + dy;

                if (battlefield->isInside(newX, newY))
                {
                    if (battlefield->isEmpty(newX, newY))
                    {
                        possibleMovesWithoutRobots.push_back(make_pair(newX, newY));
                    }
                    else
                    {
                        possibleMovesWithRobots.push_back(make_pair(newX, newY));
                    }
                }
            }
        }

        if (!possibleMovesWithRobots.empty())
        {
            int randomIndex = rand() % possibleMovesWithRobots.size();
            int steppedX = possibleMovesWithRobots[randomIndex].first;
            int steppedY = possibleMovesWithRobots[randomIndex].second;

            Robot* targetRobot = battlefield->getRobotAt(steppedX, steppedY);
            if (targetRobot)
            {
                out << getID() << " stepped on " << targetRobot->getID() << " at (" << steppedX << ", " << steppedY << ")\n";
                this->incKills();
                targetRobot->decLife();
                targetRobot->nowDead();

                if (targetRobot->isAlive())
                {
                    queueWaitingRobots.enqueue(targetRobot);
                    Robot* r = queueWaitingRobots.front();
                    battlefield->moveRobot(steppedX, steppedY, -1, -1);
                    targetRobot->setX(-1);
                    targetRobot->setY(-1);
                    out << "\n!----------------------- Robot Hit -----------------------!\n";
                    targetRobot->getDetail(out);
                    out << "\n!---------------------------------------------------------!\n";
                }
                else
                {
                    queueDestroyedRobots.enqueue(targetRobot);
                    Robot* r = queueDestroyedRobots.front();
                    battlefield->moveRobot(steppedX, steppedY, -2, -2);
                    targetRobot->setX(-2);
                    targetRobot->setY(-2);
                    out << "\n!----------------------- Robot Died -----------------------!\n\n";
                    out << targetRobot->getID() << " ran out of lives\n";
                    targetRobot->getDetail(out);
                    out << "\n!---------------------------------------------------------!\n";

                }
            }

            battlefield->moveRobot(x(), y(), steppedX, steppedY);
            setX(steppedX);
            setY(steppedY);
        }
        else if (!possibleMovesWithoutRobots.empty())
        {
            int randomIndex = rand() % possibleMovesWithoutRobots.size();
            int newX = possibleMovesWithoutRobots[randomIndex].first;
            int newY = possibleMovesWithoutRobots[randomIndex].second;

            battlefield->moveRobot(x(), y(), newX, newY);
            setX(newX);
            setY(newY);

            out << getID() << " stepped to (" << newX << ", " << newY << ")\n";
        }
        else
        {
            out << getID() << " has no valid moves available\n";
        }
    }

    void actions(Queue<Robot*>& queueWaitingRobots, Queue<Robot*>& queueDestroyedRobots, List<Robot*> listRobots, ostream& out) override
    {
        actionLook(out);
        actionFire(queueWaitingRobots, queueDestroyedRobots, listRobots, out);
        actionStep(queueWaitingRobots, queueDestroyedRobots, listRobots, out);
        out << endl << "Robot Update:";
        getDetail(out);
        out << endl;
    }
};


class Terminator : virtual  public SeeingRobot, virtual public SteppingRobot
{
protected:
    Battlefield* battlefield;

public:
    Terminator(int x, int y, string type, string alias, string IDN, string name, int kills, int totalKills, List<Robot*>& listRobots, Battlefield* battlefield)
        : Robot(x, y, type, alias, IDN, name, kills, totalKills, listRobots), battlefield(battlefield),
          SeeingRobot(x, y, type, alias, IDN, name, kills, totalKills, listRobots, battlefield),
          SteppingRobot(x, y, type, alias, IDN, name, kills, totalKills, listRobots, battlefield) {robotAutoIncInt_++;}

    static int robotAutoIncInt_;

    Robot* upgrade(List<Robot*>& listRobots,  ostream& out) override
    {
        Terminator::robotAutoIncInt_--;
        out << endl << getFullName() << " HAS BEEN UPGRADED TO (TerminatorRoboCop) with ID: TR";

        int tempIDN = TerminatorRoboCop::robotAutoIncInt_ + 1;
        string newIDN;

        if(tempIDN<10)
            newIDN = "0" + to_string(tempIDN);
        else
            newIDN = to_string(tempIDN);

        out << newIDN << getName() << endl;
        this->setFullName("TR", newIDN, getName());

        Robot* upgradedRobot = new TerminatorRoboCop(x(), y(), "TerminatorRoboCop", "TR", newIDN, getName(), getKills(), getTotalKills(), listRobots, battlefield);
        return upgradedRobot;
    }

    void actionLook(ostream& out) override
    {
        out << "\nSeeingRobot actionLook\n\n";
        out << "Surrounding positions:\n";

        int minX = max(0, x() - 1);
        int maxX = min(battlefield->getCols() - 1, x() + 1);
        int minY = max(0, y() - 1);
        int maxY = min(battlefield->getRows() - 1, y() + 1);

        for (int newY = minY; newY <= maxY; ++newY)
        {
            for (int newX = minX; newX <= maxX; ++newX)
            {
                if (newX == x() && newY == y()) continue;

                out << "(" << newX << ", " << newY << "): ";

                if (battlefield->isInside(newX, newY))
                {
                    if (battlefield->isEmpty(newX, newY))
                    {
                        out << "Empty";
                    }
                    else
                    {
                        Robot* robot = battlefield->getRobotAt(newX, newY);
                        if (robot)
                        {
                            out << "Occupied by " << robot->getID();
                        }
                    }
                }
                else
                {
                    out << "Outside of battlefield.";
                }
                out << "\n";
            }
        }
    }

    void actionStep(Queue<Robot*>& queueWaitingRobots, Queue<Robot*>& queueDestroyedRobots, List<Robot*> listRobots, ostream& out) override
    {
        out << "\nSteppingRobot actionStep\n\n";

        Vector<pair<int, int>> possibleMovesWithRobots;
        Vector<pair<int, int>> possibleMovesWithoutRobots;

        for (int dx = -1; dx <= 1; ++dx)
        {
            for (int dy = -1; dy <= 1; ++dy)
            {
                if (dx == 0 && dy == 0) continue;

                int newX = x() + dx;
                int newY = y() + dy;

                if (battlefield->isInside(newX, newY))
                {
                    if (battlefield->isEmpty(newX, newY))
                    {
                        possibleMovesWithoutRobots.push_back(make_pair(newX, newY));
                    }
                    else
                    {
                        possibleMovesWithRobots.push_back(make_pair(newX, newY));
                    }
                }
            }
        }

        if (!possibleMovesWithRobots.empty())
        {
            int randomIndex = rand() % possibleMovesWithRobots.size();
            int steppedX = possibleMovesWithRobots[randomIndex].first;
            int steppedY = possibleMovesWithRobots[randomIndex].second;

            Robot* targetRobot = battlefield->getRobotAt(steppedX, steppedY);
            if (targetRobot)
            {
                out << getID() << " stepped on " << targetRobot->getID() << " at (" << steppedX << ", " << steppedY << ")\n";
                this->incKills();
                targetRobot->decLife();
                targetRobot->nowDead();

                if (targetRobot->isAlive())
                {
                    queueWaitingRobots.enqueue(targetRobot);
                    Robot* r = queueWaitingRobots.front();
                    battlefield->moveRobot(steppedX, steppedY, -1, -1);
                    targetRobot->setX(-1);
                    targetRobot->setY(-1);
                    out << "\n!----------------------- Robot Hit -----------------------!\n";
                    targetRobot->getDetail(out);
                    out << "\n!---------------------------------------------------------!\n";
                }
                else
                {
                    queueDestroyedRobots.enqueue(targetRobot);
                    Robot* r = queueDestroyedRobots.front();
                    battlefield->moveRobot(steppedX, steppedY, -2, -2);
                    targetRobot->setX(-2);
                    targetRobot->setY(-2);
                    out << "\n!----------------------- Robot Died -----------------------!\n\n";
                    out << targetRobot->getID() << " ran out of lives\n";
                    targetRobot->getDetail(out);
                    out << "\n!---------------------------------------------------------!\n";

                }
            }

            battlefield->moveRobot(x(), y(), steppedX, steppedY);
            setX(steppedX);
            setY(steppedY);
        }
        else if (!possibleMovesWithoutRobots.empty())
        {
            int randomIndex = rand() % possibleMovesWithoutRobots.size();
            int newX = possibleMovesWithoutRobots[randomIndex].first;
            int newY = possibleMovesWithoutRobots[randomIndex].second;

            battlefield->moveRobot(x(), y(), newX, newY);
            setX(newX);
            setY(newY);

            out << getID() << " stepped to (" << newX << ", " << newY << ")\n";
        }
        else
        {
            out << getID() << " has no valid moves available\n";
        }
    }

    void actions(Queue<Robot*>& queueWaitingRobots, Queue<Robot*>& queueDestroyedRobots, List<Robot*> listRobots, ostream& out) override
    {
        actionLook(out);
        actionStep(queueWaitingRobots, queueDestroyedRobots, listRobots, out);
        out << endl << "Robot Update:";
        getDetail(out);
        out << endl;
    }
};


class RoboCop : virtual public SeeingRobot, virtual public MovingRobot, virtual public ShootingRobot
{
protected:
    Battlefield* battlefield;

public:
    RoboCop(int x, int y, string type, string alias, string IDN, string name, int kills, int totalKills, List<Robot*>& listRobots, Battlefield* battlefield)
        : Robot(x, y, type, alias, IDN, name, kills, totalKills, listRobots), battlefield(battlefield),
          SeeingRobot(x, y, type, alias, IDN, name, kills, totalKills, listRobots, battlefield),
          MovingRobot(x, y, type, alias, IDN, name, kills, totalKills, listRobots, battlefield),
          ShootingRobot(x, y, type, alias, IDN, name, kills, totalKills, listRobots, battlefield) {robotAutoIncInt_++;}

    static int robotAutoIncInt_;

    Robot* upgrade(List<Robot*>& listRobots,  ostream& out) override
    {
        RoboCop::robotAutoIncInt_--;
        out << endl << getFullName() << " HAS BEEN UPGRADED TO (TerminatorRoboCop) with ID: TR";

        int tempIDN = TerminatorRoboCop::robotAutoIncInt_ + 1;
        string newIDN;

        if(tempIDN<10)
            newIDN = "0" + to_string(tempIDN);
        else
            newIDN = to_string(tempIDN);

        out << newIDN << getName() << endl;
        this->setFullName("TR", newIDN, getName());

        Robot* upgradedRobot = new TerminatorRoboCop(x(), y(), "TerminatorRoboCop", "TR", newIDN, getName(), getKills(), getTotalKills(), listRobots, battlefield);
        return upgradedRobot;
    }

    void actionLook(ostream& out) override
    {
        out << "\nSeeingRobot actionLook\n\n";
        out << "Surrounding positions:\n";

        int minX = max(0, x() - 1);
        int maxX = min(battlefield->getCols() - 1, x() + 1);
        int minY = max(0, y() - 1);
        int maxY = min(battlefield->getRows() - 1, y() + 1);

        for (int newY = minY; newY <= maxY; ++newY)
        {
            for (int newX = minX; newX <= maxX; ++newX)
            {
                if (newX == x() && newY == y()) continue;

                out << "(" << newX << ", " << newY << "): ";

                if (battlefield->isInside(newX, newY))
                {
                    if (battlefield->isEmpty(newX, newY))
                    {
                        out << "Empty";
                    }
                    else
                    {
                        Robot* robot = battlefield->getRobotAt(newX, newY);
                        if (robot)
                        {
                            out << "Occupied by " << robot->getID();
                        }
                    }
                }
                else
                {
                    out << "Outside of battlefield.";
                }
                out << "\n";
            }
        }
    }

    void actionMove(ostream& out) override
    {
        out << "\nMovingRobot actionMove\n\n";

        Vector<pair<int, int>> possibleMoves;

        int minX = max(0, x() - 1);
        int maxX = min(battlefield->getCols() - 1, x() + 1);
        int minY = max(0, y() - 1);
        int maxY = min(battlefield->getRows() - 1, y() + 1);

        for (int newX = minX; newX <= maxX; ++newX)
        {
            for (int newY = minY; newY <= maxY; ++newY)
            {
                if (newX == x() && newY == y()) continue;

                if (battlefield->isInside(newX, newY) && battlefield->isEmpty(newX, newY))
                {
                    possibleMoves.push_back(make_pair(newX, newY));
                }
            }
        }

        if (!possibleMoves.empty())
        {
            int randomIndex = rand() % possibleMoves.size();
            int newX = possibleMoves[randomIndex].first;
            int newY = possibleMoves[randomIndex].second;

            battlefield->moveRobot(x(), y(), newX, newY);
            setX(newX);
            setY(newY);

            out << "Robot " << getID() << " moved to (" << newX << ", " << newY << ")\n";
        }
        else
        {
            out << "No available move for " << getID() << endl;
        }
    }

    void actionFire(Queue<Robot*>& queueWaitingRobots, Queue<Robot*>& queueDestroyedRobots, List<Robot*> listRobots, ostream& out) override
    {
        out << "\nShootingRobot actionFire\n";

        for (int i = 0; i < 3; i++)
        {
            const int maxDistance = 10;
            int targetX, targetY;
            int negX= rand()%2;
            int negY= rand()%2;
            int randX= rand()%11;
            int randY= rand()%(11-randX);

            if (negX == 0 && negY == 0)
            {
                targetX = x() + randX;
                targetY = y() + randY;
            }
            else if (negX == 1 && negY == 0)
            {
                targetX = x() - randX;
                targetY = y() + randY;
            }
            else if (negX == 0 && negY == 1)
            {
                targetX = x() + randX;
                targetY = y() - randY;
            }
            else
            {
                targetX = x() - randX;
                targetY = y() - randY;
            }

            if (targetX < 0)
                targetX = - targetX;
            if (targetY < 0)
                targetY = - targetY;

            if (targetX > battlefield->getCols()-2)
                targetX = randX - 0.5*x();
            if (targetY > battlefield->getRows()-2)
                targetY = randY - 0.5*y();

            if (targetX < 0)
                targetX = - targetX;
            if (targetY < 0)
                targetY = - targetY;

            if (targetX == x() && targetY == y())
                targetX++;

            out << endl << getID() << " fired at (" << targetX << ", " << targetY << ")\n";

            Robot* targetRobot = battlefield->getRobotAt(targetX, targetY);
            if (targetRobot)
            {
                out << getID() << " hit " << targetRobot->getID() << " at (" << targetX << ", " << targetY << ")\n";
                this->incKills();
                targetRobot->decLife();
                targetRobot->nowDead();

                if (targetRobot->isAlive())
                {
                    queueWaitingRobots.enqueue(targetRobot);
                    Robot* r = queueWaitingRobots.front();
                    battlefield->moveRobot(targetX, targetY, -1, -1);
                    targetRobot->setX(-1);
                    targetRobot->setY(-1);
                    out << "\n!----------------------- Robot Hit -----------------------!\n";
                    targetRobot->getDetail(out);
                    out << endl << "\n!---------------------------------------------------------!\n";
                }
                else
                {
                    queueDestroyedRobots.enqueue(targetRobot);
                    Robot* r = queueDestroyedRobots.front();
                    battlefield->moveRobot(targetX, targetY, -2, -2);
                    targetRobot->setX(-2);
                    targetRobot->setY(-2);
                    out << "\n!----------------------- Robot Died -----------------------!\n\n";
                    out << targetRobot->getID() << " ran out of lives\n";
                    targetRobot->getDetail(out);
                    out << endl << "\n!---------------------------------------------------------!\n";

                }
            }
            else
            {
                out << "No robot hit\n";
            }
        }
    }

    void actions(Queue<Robot*>& queueWaitingRobots, Queue<Robot*>& queueDestroyedRobots, List<Robot*> listRobots, ostream& out) override
    {
        actionLook(out);
        actionMove(out);
        actionFire(queueWaitingRobots, queueDestroyedRobots, listRobots, out);
        out << endl << "Robot Update:";
        getDetail(out);
        out << endl;
    }
};


int UltimateRobot::robotAutoIncInt_ = 0;
int RoboTank::robotAutoIncInt_ = 0;
int Madbot::robotAutoIncInt_ = 0;
int BlueThunder::robotAutoIncInt_ = 0;
int TerminatorRoboCop::robotAutoIncInt_ = 0;
int Terminator::robotAutoIncInt_ = 0;
int RoboCop::robotAutoIncInt_ = 0;


int main()
{
    srand(1211110222); //SEED NUMBER = GROUP LEADER'S STUDENT ID
    Robot* robot = nullptr;
    Battlefield* simulation = nullptr;

    string line;
    int noLine = 1;
    int rows = 0, cols = 0, turn = 0, turns = 0, i = 0, robots = 0;
    List<Robot*> listRobots;
    Queue<Robot*> queueDestroyedRobots;
    Queue<Robot*> queueWaitingRobots;

    ifstream input("fileInput2a.txt");
    ofstream out("fileOutput2a.txt");

    if (input.is_open())
    {
        out << "Read input.txt:" << endl;
        while (getline(input, line))
        {
            out << line << endl;
        }
        out << endl;
        input.clear();
        input.seekg(0, ios::beg);

        while (getline(input, line))
        {
            if (noLine <= 3)
            {
                string valueFromLine = line.substr(line.find(":") + 1);
                stringstream temp(valueFromLine);
                string First, Second;
                temp >> First >> Second;

                if (noLine == 1)
                {
                    rows = stoi(Second);
                    cols = stoi(First);
                }
                if (noLine == 2)
                {
                    turns = stoi(First);
                }
                if (noLine == 3)
                {
                    robots = stoi(First);
                }
                noLine++;
            }
        }
    }

    input.clear();
    input.seekg(0, ios::beg);
    noLine = 1;

    Battlefield battlefield(rows, cols);

    if (input.is_open())
    {
        while (getline(input, line))
        {
            if (noLine > 3)
            {
                stringstream temp(line);
                int Kills = 0;
                int TotalKills = 0;
                string Type, fName, strX, strY;
                temp >> Type >> fName >> strX >> strY;

                int x = (strX == "random") ? rand() % cols : stoi(strX);
                int y = (strY == "random") ? rand() % rows : stoi(strY);

                string Alias = fName.substr(0, 2);
                string IDN = fName.substr(2, 2);
                string Name = fName.substr(4);

                if (Type == "Terminator")
                {
                    Alias = fName.substr(0, 1);
                    IDN = fName.substr(1, 2);
                    Name = fName.substr(3);
                }

                if (Type == "RoboCop")
                {
                    robot = new RoboCop(x, y, Type, Alias, IDN, Name, Kills, TotalKills, listRobots, &battlefield);
                }
                else if (Type == "Terminator")
                {
                    robot = new Terminator(x, y, Type, Alias, IDN, Name, Kills, TotalKills, listRobots, &battlefield);
                }
                else if (Type == "TerminatorRoboCop")
                {
                    robot = new TerminatorRoboCop(x, y, Type, Alias, IDN, Name, Kills, TotalKills, listRobots, &battlefield);
                }
                else if (Type == "BlueThunder")
                {
                    robot = new BlueThunder(x, y, Type, Alias, IDN, Name, Kills, TotalKills, listRobots, &battlefield);
                }
                else if (Type == "Madbot")
                {
                    robot = new Madbot(x, y, Type, Alias, IDN, Name, Kills, TotalKills, listRobots, &battlefield);
                }
                else if (Type == "RoboTank")
                {
                    robot = new RoboTank(x, y, Type, Alias, IDN, Name, Kills, TotalKills, listRobots, &battlefield);
                }
                else if (Type == "UltimateRobot")
                {
                    robot = new UltimateRobot(x, y, Type, Alias, IDN, Name, Kills, TotalKills, listRobots, &battlefield);
                }

                if (robot)
                {
                    battlefield.placeRobot(x, y, robot, out);
                    listRobots.push_back(robot);
                }
            }
            noLine++;
        }
        input.close();
    }
    else
    {
        out << "Unable to open file";
    }

    while(turn <= turns && i < listRobots.size())
    {
        if (turn == 0)
        {
            for (Robot* robot : listRobots)
            {
                out << *robot << " at (" << robot->x() << ", " << robot->y() << ")" << endl;
            }
            out << endl;
            battlefield.display(out);
            out << listRobots.get(i)->getQ(queueWaitingRobots, queueDestroyedRobots, out);
            turn++;
        }
        else
        {
            if (!queueWaitingRobots.empty())
            {
                out << "\n\n------------------------ Announcement for Turn " << turn << " ------------------------\n\n";
                Robot* spawn = queueWaitingRobots.front();
                int newX, newY;
                do {
                    newX = rand() % cols;
                    newY = rand() % rows;
                } while (!battlefield.isEmpty(newX, newY));

                battlefield.moveRobot(-1, -1, newX, newY);
                spawn->setX(newX);
                spawn->setY(newY);
                queueWaitingRobots.dequeue();
                out << spawn->getFullName() << " spawned in at (" << newX << "," << newY << ")";
                spawn->nowLive();
            }
            out << "\n\n-------------------------------------------------------------------------\n";
            out << "\nTurn " << turn << ":\n";
            int turnD = turn;
            while (turn == turnD)
            {
                if (!listRobots.get(i)->isDead())
                {
                    out << "_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ Robots Details _ _ _ _ _ _ _ _ _ _ _ _ _ _ _\n";
                    for (int a = 0; a < listRobots.size(); a++)
                    {
                        listRobots.get(a)->getDetail(out);
                        out << " | Dead?: " << listRobots.get(a)->isDead();
                    }

                    out << "\n\nROBOCOP COUNT: " << RoboCop::robotAutoIncInt_ << endl;
                    out << "TERMINATOR COUNT: " << Terminator::robotAutoIncInt_ << endl;
                    out << "TERMINATORROBOCOP COUNT: " << TerminatorRoboCop::robotAutoIncInt_ << endl;
                    out << "BLUETHUNDER COUNT: " << BlueThunder::robotAutoIncInt_ << endl;
                    out << "MADBOT COUNT: " << Madbot::robotAutoIncInt_ << endl;
                    out << "ROBOTANK COUNT: " << RoboTank::robotAutoIncInt_ << endl;
                    out << "ULTIMATEROBOT COUNT: " << UltimateRobot::robotAutoIncInt_ << endl;

                    out<< "\n_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _";

                    out << endl << endl << *listRobots.get(i) << " at (" << listRobots.get(i)->x() << "," << listRobots.get(i)->y() << ")" << endl;
                    listRobots.get(i)->actions(queueWaitingRobots, queueDestroyedRobots, listRobots, out);

                    if (!(listRobots.get(i)->getType()=="UltimateRobot"))
                    {
                        if(listRobots.get(i)->getKills() == 3)
                        {
                            listRobots.get(i)->resetKills();
                            int tempX = listRobots.get(i)->x();
                            int tempY = listRobots.get(i)->y();
                            battlefield.moveRobot(listRobots.get(i)->x(), listRobots.get(i)->y(), -3, -3);
                            listRobots.get(i)->setX(-3);
                            listRobots.get(i)->setY(-3);
                            Robot* upgradedRobot = listRobots.get(i)->upgrade(listRobots, out);
                            listRobots.set(i, upgradedRobot);
                            upgradedRobot->setX(tempX);
                            upgradedRobot->setY(tempY);
                            battlefield.placeRobot(upgradedRobot->x(), upgradedRobot->y(), upgradedRobot, out);
                        }
                    }
                    turn++;
                    out << endl;
                    battlefield.display(out);

                    out << listRobots.get(i)->getQ(queueWaitingRobots, queueDestroyedRobots, out);

                    if (queueDestroyedRobots.size() == robots-1)
                    {
                        out << "\n----------------------------------------------------------------------------------------\n";
                        out << "\nALL ROBOTS DESTROYED, EXCEPT ONE. THE GAME FINISHED EARLY. WINNER IS " << *listRobots.get(i) << endl;
                        out << "\n----------------------------------------------------------------------------------------\n";
                        return 0;
                    }
                }
                i++;
                if (i >= listRobots.size())
                {
                    i = 0;
                }
            }
        }
    }
    out << "\n----------------------------------------------------------------------------------------\n";
    out << "\nMAXIMUM TURNS REACHED. NO ROBOT WON." << endl;
    out << "\n----------------------------------------------------------------------------------------\n";
    return 0;
    out.close();
}
