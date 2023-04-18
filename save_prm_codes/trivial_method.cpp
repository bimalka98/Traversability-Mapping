
struct state_t{
    double x[3]; //  1 - x, 2 - y, 3 - z
    float theta;
    int stateId;
    float cost;
    bool validFlag;
    // # Cost types
    // # 0. obstacle cost
    // # 1. elevation cost
    // # 2. distance cost
    float costsToRoot[NUM_COSTS];    

    state_t* parentState; // parent for this state in PRM and RRT*
    vector<neighbor_t> neighborList; // PRM adjencency list with edge costs
    
    // Following variables are not used: https://github.com/TixiaoShan/traversability_mapping/issues/7#issuecomment-612863737
    // float costsToParent[NUM_COSTS]; // used in RRT*
    // float costsToGo[NUM_COSTS];
    // vector<state_t*> childList; // RRT*

    // default initialization
    state_t(){
        parentState = NULL;
        for (int i = 0; i < NUM_COSTS; ++i){
            costsToRoot[i] = FLT_MAX;
            // costsToParent[i] = FLT_MAX;
            // costsToGo[i] = FLT_MAX;
        }
    }
    // use a state input to initialize new state
    
    state_t(state_t* stateIn){
        // pose initialization
        for (int i = 0; i < 3; ++i)
            x[i] = stateIn->x[i];
        theta = stateIn->theta;
        // regular initialization
        parentState = NULL;
        for (int i = 0; i < NUM_COSTS; ++i){
            costsToRoot[i] = FLT_MAX;
            // costsToParent[i] = stateIn->costsToParent[i];
        }
    }
};


struct neighbor_t{
    state_t* neighbor;
    float edgeCosts[NUM_COSTS]; // the cost from this state to neighbor
    neighbor_t(){
        neighbor = NULL;
        for (int i = 0; i < NUM_COSTS; ++i)
            edgeCosts[i] = FLT_MAX;
    }
};

/*
Method 1: Trivial method
*/

#include <fstream> // for file I/O
#include <iostream> // for console I/O
#include <string> // for strings
#include <vector> // for vectors

using namespace std;

void saveNodeList(const vector<state_t*>& nodeList, const string& filename) {
    ofstream outFile(filename, ios::out | ios::binary); // create a new binary file for output
    if (!outFile) {
        cerr << "Error: could not open output file " << filename << endl;
        return;
    }

    size_t numStates = nodeList.size();
    outFile.write(reinterpret_cast<const char*>(&numStates), sizeof(numStates)); // write the number of states to the file

    for (size_t i = 0; i < numStates; i++) {
        const state_t& state = *(nodeList[i]);

        // write the state's member variables to the file
        outFile.write(reinterpret_cast<const char*>(state.x), sizeof(state.x));
        outFile.write(reinterpret_cast<const char*>(&state.theta), sizeof(state.theta));
        outFile.write(reinterpret_cast<const char*>(&state.stateId), sizeof(state.stateId));
        outFile.write(reinterpret_cast<const char*>(&state.cost), sizeof(state.cost));
        outFile.write(reinterpret_cast<const char*>(&state.validFlag), sizeof(state.validFlag));
        outFile.write(reinterpret_cast<const char*>(state.costsToRoot), sizeof(state.costsToRoot));

        // write the parent state's index in the nodeList
        size_t parentIndex = numStates; // default to an invalid index
        if (state.parentState != nullptr) {
            for (size_t j = 0; j < numStates; j++) {
                if (nodeList[j] == state.parentState) {
                    parentIndex = j;
                    break;
                }
            }
        }
        outFile.write(reinterpret_cast<const char*>(&parentIndex), sizeof(parentIndex));

        // write the number of neighbors and each neighbor's member variables to the file
        size_t numNeighbors = state.neighborList.size();
        outFile.write(reinterpret_cast<const char*>(&numNeighbors), sizeof(numNeighbors));
        for (size_t j = 0; j < numNeighbors; j++) {
            const neighbor_t& neighbor = state.neighborList[j];
            size_t neighborIndex = numStates; // default to an invalid index
            for (size_t k = 0; k < numStates; k++) {
                if (nodeList[k] == neighbor.neighbor) {
                    neighborIndex = k;
                    break;
                }
            }
            outFile.write(reinterpret_cast<const char*>(&neighborIndex), sizeof(neighborIndex));
            outFile.write(reinterpret_cast<const char*>(neighbor.edgeCosts), sizeof(neighbor.edgeCosts));
        }
    }

    outFile.close();
    cout << "Saved PRM graph with " << numStates << " states to file " << filename << endl;
}


vector<state_t*> loadNodeList(const string& filename) {
    vector<state_t*> nodeList;

    ifstream inFile(filename, ios::in | ios::binary); // open the input file for reading
    if (!inFile) {
        cerr << "Error: could not open input file " << filename << endl;
        return nodeList;
    }

    size_t numStates;
    inFile.read(reinterpret_cast<char*>(&numStates), sizeof(numStates)); // read the number of states from the file

    nodeList.resize(numStates, nullptr); // reserve space for the state pointers

    for (size_t i = 0; i < numStates; i++) {
        state_t* state = new state_t;

        // read the state's member variables from the file
        inFile.read(reinterpret_cast<char*>(state->x), sizeof(state->x));
        inFile.read(reinterpret_cast<char*>(&state->theta), sizeof(state->theta));
        inFile.read(reinterpret_cast<char*>(&state->stateId), sizeof(state->stateId));
        inFile.read(reinterpret_cast<char*>(&state->cost), sizeof(state->cost));
        inFile.read(reinterpret_cast<char*>(&state->validFlag), sizeof(state->validFlag));
        inFile.read(reinterpret_cast<char*>(state->costsToRoot), sizeof(state->costsToRoot));

        // read the parent state's index in the nodeList
        size_t parentIndex;
        inFile.read(reinterpret_cast<char*>(&parentIndex), sizeof(parentIndex));
        if (parentIndex < numStates) {
            state->parentState = nodeList[parentIndex];
        }

        // read the number of neighbors and each neighbor's member variables from the file
        size_t numNeighbors;
        inFile.read(reinterpret_cast<char*>(&numNeighbors), sizeof(numNeighbors));
        for (size_t j = 0; j < numNeighbors; j++) {
            neighbor_t neighbor;

            size_t neighborIndex;
            inFile.read(reinterpret_cast<char*>(&neighborIndex), sizeof(neighborIndex));
            if (neighborIndex < numStates) {
                neighbor.neighbor = nodeList[neighborIndex];
            }

            inFile.read(reinterpret_cast<char*>(neighbor.edgeCosts), sizeof(neighbor.edgeCosts));

            state->neighborList.push_back(neighbor);
        }

        nodeList[i] = state;
    }

    inFile.close();
    cout << "Loaded PRM graph with " << numStates << " states from file " << filename << endl;
    return nodeList;
}


/// Load a PRM graph from a file: corresponding to te method developed by me.

vector<state_t*> loadPRMGraph(string fileName) {
    ifstream inFile(fileName, std::ios::in | std::ios::binary);
    if (!inFile.is_open()) {
        std::cerr << "Error: could not open input file " << fileName << std::endl;
        return {};
    }

    // read the number of states in the graph
    size_t numStates;
    inFile.read(reinterpret_cast<char*>(&numStates), sizeof(numStates));

    // create a vector to hold the states
    vector<state_t*> nodeList(numStates);

    for (size_t i = 0; i < numStates; i++) {
        // create a new state and read its member variables from the file
        state_t* state = new state_t;
        inFile.read(reinterpret_cast<char*>(state->x), sizeof(state->x));
        inFile.read(reinterpret_cast<char*>(&state->theta), sizeof(state->theta));
        inFile.read(reinterpret_cast<char*>(&state->stateId), sizeof(state->stateId));
        inFile.read(reinterpret_cast<char*>(&state->cost), sizeof(state->cost));
        inFile.read(reinterpret_cast<char*>(&state->validFlag), sizeof(state->validFlag));
        inFile.read(reinterpret_cast<char*>(state->costsToRoot), sizeof(state->costsToRoot));

        // read the parent state ID from the file
        int parentStateId;
        inFile.read(reinterpret_cast<char*>(&parentStateId), sizeof(parentStateId));

        // set the state's parent based on the parent state ID
        if (parentStateId >= 0 && parentStateId < static_cast<int>(numStates)) {
            state->parentState = nodeList[parentStateId];
        }

        // read the number of neighbors for the state and create a neighbor list
        size_t numNeighbors;
        inFile.read(reinterpret_cast<char*>(&numNeighbors), sizeof(numNeighbors));
        state->neighborList.resize(numNeighbors);

        // read each neighbor's state ID and edge costs and add it to the neighbor list
        int neighborIndex;
        for (size_t j = 0; j < numNeighbors; j++) {
            inFile.read(reinterpret_cast<char*>(&neighborIndex), sizeof(neighborIndex));
            if (neighborIndex >= 0 && neighborIndex < static_cast<int>(numStates)) {
                state->neighborList[j].neighbor = nodeList[neighborIndex];
                inFile.read(reinterpret_cast<char*>(state->neighborList[j].edgeCosts), sizeof(state->neighborList[j].edgeCosts));
            }
        }

        // add the state to the node list
        nodeList[i] = state;
    }

    inFile.close();
    return nodeList;
}

// save prm finalized
    void savePRMGraph(){
        std::cout << "==================" << std::endl;
        std::cout << "|Saving PRM graph|" << std::endl;
        std::cout << "==================" << std::endl;

        std::ofstream outFile(prmFileName, std::ios::out | std::ios::binary); // create a new binary file for output
        if (!outFile.is_open()) {
            std::cerr << "Error: could not open output file " << prmFileName << std::endl;
            return;
        }

        size_t numStates = nodeList.size();
        outFile.write(reinterpret_cast<const char*>(&numStates), sizeof(numStates)); // write the number of states to the file
        std::cout << "saving " << numStates << " states to " << prmFileName << std::endl;

        for (size_t i = 0; i < numStates; i++) {
            
            // get a reference to the current state
            const state_t& state = *(nodeList[i]);

            std::cout << "saving state " << state.stateId << " of " << (numStates-1) << std::endl;

            // write the state's member variables to the file
            outFile.write(reinterpret_cast<const char*>(state.x), sizeof(state.x));
            outFile.write(reinterpret_cast<const char*>(&state.theta), sizeof(state.theta));
            outFile.write(reinterpret_cast<const char*>(&state.stateId), sizeof(state.stateId));
            outFile.write(reinterpret_cast<const char*>(&state.cost), sizeof(state.cost));
            outFile.write(reinterpret_cast<const char*>(&state.validFlag), sizeof(state.validFlag));
            outFile.write(reinterpret_cast<const char*>(state.costsToRoot), sizeof(state.costsToRoot));
            
            std::cout << "saved member data" << std::endl; // upto here program is working fine

            // write the parent state ID to the file
            int parentStateId = -1;
            if (state.parentState != nullptr) {
                parentStateId = state.parentState->stateId;
                std::cout << "parent state ID: " << parentStateId << std::endl;
                outFile.write(reinterpret_cast<const char*>(&parentStateId), sizeof(parentStateId));
            }
            else {

                // at the reloading we need to know if there is no parent for a state
                parentStateId = -1;
                std::cerr << "Error: no parent exist for this state, saved as -1" << std::endl;
                outFile.write(reinterpret_cast<const char*>(&parentStateId), sizeof(parentStateId));
            }            

            std::cout << "saved parent state ID" << std::endl;

            // write the number of neighbors and each neighbor's member variables to the file
            size_t numNeighbors = state.neighborList.size();
            outFile.write(reinterpret_cast<const char*>(&numNeighbors), sizeof(numNeighbors));

            std::cout << "saving neighbor data..." << std::endl;

            // write the neighbor's state ID and edge costs to the file
            int neighborIndex = -1;
            for (size_t j = 0; j < numNeighbors; j++) {
                
                // get a reference to the current neighbor
                const neighbor_t& neighbor = state.neighborList[j];

                // check if the neighbor is valid: i guess it is always valid. not sure, better to check
                if (neighbor.neighbor != nullptr) {
                    neighborIndex = neighbor.neighbor->stateId;
                    std::cout << "saving neighbor " << j << " of " << (numNeighbors-1) << " with state ID " << neighborIndex << std::endl;

                    outFile.write(reinterpret_cast<const char*>(&neighborIndex), sizeof(neighborIndex));
                    outFile.write(reinterpret_cast<const char*>(neighbor.edgeCosts), sizeof(neighbor.edgeCosts));    

                } else {
                    neighborIndex = -1;
                    outFile.write(reinterpret_cast<const char*>(&neighborIndex), sizeof(neighborIndex));
                    std::cerr << "Error: neighbor is null, saved as -1" << std::endl;
                }
            }
        }

        outFile.close();
        std::cout << "Saved PRM graph with " << numStates << " states to file " << prmFileName << std::endl;

    }