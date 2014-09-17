#include <iostream>
#include <vector>

using namespace std;

#ifndef  NULL
#define NULL 0
#endif

const int ACTNUM = 5;
const int MISS_CANN_COUNTS = 3;

int actions[ACTNUM][2] = 
{
    {0,1},
    {1,0},
    {0,2},
    {1,1},
    {2,0},


};

class Node {
    public:
        int numMiss;
        int numCann;
        //-1 for boat on left, 1 for boat on right
        int boatPos;
        //five possible actions
        vector<Node*>* children;
        Node* parent;

        /* Node
         * pre: takes number of missionaries, cannibals, boat position, and parent
         * post: returns new Node
         */
        Node(int, int, int, Node*);

        /* equals
         * pre: takes this and node
         * post: returns true if nodes are equal
         */
        bool equals(Node* node);

        /* checkAction
         * pre: takes action vector, current node, and history vector
         * post: returns true if action is legal
         */
        bool checkAction(int[], Node*, vector<Node*>*);

        /* addChild
         * pre: takes this and child
         * post: returns true if child successfully added
         */
        bool addChild(Node* child);

        /* checkHistory
         * pre: takes new number of missionaries, cannibals, and history vector
         * pre: returns true if pair of missionaries and cannibals does not exist in history
         */
        bool checkHistory(int, int, vector<Node*>*);
};

Node::Node(int missStart, int cannStart, int boatLoc, Node* par)
{
    numMiss = missStart;
    numCann = cannStart;
    boatPos = boatLoc;
    children = new vector<Node*>();
    parent = par;
}

/* bfs
 * pre: takes top level state and goal state
 * post: returns node that equals goal state
 */
Node* bfs(Node* node, Node* goal)
{
    int findex = 0;
    int i;
    vector<Node*>* frontier = new vector<Node*>();
    Node* curr;
    
    //Add first node
    frontier->push_back(node);

    if (node == NULL || goal == NULL)
        return NULL;
    
    while (findex < frontier->size())
    {
        curr = frontier->at(findex); 
         
        //Is the left side of the river empty
        if (curr->equals(goal))
            return curr;
        
        //For all possible actions
        for (i=0; i<ACTNUM; i++)
        {
            //Is the action legal?
            if(curr->checkAction(actions[i], node, frontier))
            {
                //Create a new node with changes to the counts and boat position
                Node* newNode = new Node(
                        curr->numMiss + (curr->boatPos * actions[i][0]),
                        curr->numCann + (curr->boatPos * actions[i][1]),
                        (curr->boatPos - (2 * curr->boatPos)),
                        curr);

                //add new node to tree
                curr->addChild(newNode);
                
                //add new node to frontier to be checked
                frontier->push_back(newNode);
            }
        }
        findex++;
    }

    delete(frontier);

    return NULL;
}

bool Node::equals(Node* node)
{
    if ((this->numMiss == node->numMiss) &&
            (this->numCann == node->numCann))
        return true;

    return false;
}

bool Node::checkAction(int act[], Node* orig, vector<Node*>* frontier)
{
    int newMiss = this->numMiss + (this->boatPos * act[0]);
    int newCann = this->numCann + (this->boatPos * act[1]);
   
    //Don't send 1 person across if the boat is on the left (start) side
    if (this->boatPos == -1 && ( act==actions[0] || act==actions[1] ))
        return false;
    
    int rightNewMiss = orig->numMiss - newMiss;
    int rightNewCann = orig->numCann - newCann;

    //if missionaries +- new > cannibals +- new
    if ((newMiss > MISS_CANN_COUNTS) || (newMiss < 0) || (newCann > MISS_CANN_COUNTS) || (newCann < 0))
        return false;

    //if (newMiss >= newCann && rightNewMiss >= rightNewCann)
    if ((newMiss >= newCann) || (newMiss == 0) && checkHistory(newMiss, newCann, frontier))
        return true;
    
    return false;
}

bool Node::checkHistory(int newMiss, int newCann, vector<Node*>* frontier)
{
    int i;

    for (i=frontier->size()-1; i>=0; i--)
    {
        if ((newMiss == frontier->at(i)->numMiss) && (newCann == frontier->at(i)->numCann) && (frontier->at(i)->boatPos == -1))
            return false; 
    }

    return true;
}

bool Node::addChild(Node* child)
{
    this->children->push_back(child);
}

int main()
{
    //New node with 3 missionaries, 3 cannibals, and boat on the left
    Node* startNode = new Node(MISS_CANN_COUNTS, MISS_CANN_COUNTS, -1, NULL);
    //Goal state is empty
    Node* endNode = new Node(0,0,0,NULL);

    //Get result of breadth-first search
    Node* result = bfs(startNode, endNode);

    if (result == NULL)
        cout << "Failed" << endl;
    else
        cout << "Success" << endl;

    vector<Node*>* solution = new vector<Node*>();

    while (result != NULL)
    {
        solution->push_back(result);   
        //cout << result->numMiss << "," << result->numCann << endl;
        result = result->parent;
    }

    while(!solution->empty())
    {
        result = solution->back();
        cout << "Missionaries: " << result->numMiss << " |";
        if (result->boatPos == -1)
            cout << "<>\t  ";
        else
            cout << "\t<>";
        
        cout << "| " << MISS_CANN_COUNTS - result->numMiss << endl;

        cout << "Cannibals:    " << result->numCann << " |\t  | " << MISS_CANN_COUNTS - result->numCann << endl;

        cout << "-----------------" << endl;
        
        solution->pop_back();
        //solution->erase(solution->begin());
    }

    return 1;
}
