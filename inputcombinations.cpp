/*
Input combinations:
Description:
A simple program to generate all the possible groupings of n elements, in a hill-climbing approach. 

Notes: 
- The program assumes that elements are represented as chars.
- Surely there must be a more efficient implementation, but this one does the job without too much hassle.

Authors: 
akhil.podugu@st.ovgu.de
gabrielcampero@acm.org

**/


#include <iostream>
#include <vector>
#include <set>
#include <cstdlib>
#include <sstream>
#include <string>

using namespace std;

class grouping{//Class to keep together groups, and the step in which they were generated.
private:
	vector<string> groups;
	int step;
public:
	grouping(){
		step=0;
	}
	grouping(vector<string> gr, int r){
		step=r;
		groups=gr;
	}
	int grp_step(){
		return step;
	}
	vector<string> grp_groups(){
		return groups;
	}
	
};

class bell //Class that calculates all the possible groupings of n elements, in a hill-climbing approach. The class is called bell, since the number of combinations generated should always reach the bell number when steps=n.
{
private: 	
	vector<grouping> results;
	int num_steps;
public:
	bell(){
		num_steps=1;
	}

	bell(int steps){
		num_steps=(steps<1)?1:steps;//a simple check that steps should be positive
	}


	bool _equal(string v1, string v2){//Returns true if v1 is equal (save for the order of the characters) to v2
		if (v1.size()!=v2.size()){//different sizes => the strings cannot be equal
			return false;
		}
		if (v1.size()==0){
			return false; //empty strings are not equal (I know, it's a strange semantic, but seems fitting).
		}
		for (int i=0; i<v1.size(); i++){
			if (v2.find(v1.at(i))!=std::string::npos && 
			   (v2.find_first_of(v1.at(i))==v2.find_last_of(v1.at(i)))){ //If every character of v1 is found in v2, and only once
				//do nothing
			}
			else{
				return false;
			}				
		}
		return true;
	}

	bool _equal_groupings(vector<string> v1, vector<string> v2){//Returns true if v1 is equal (save for the order of the characters, or groups) to v2
		if (v1.size()!=v2.size()){//different number of groups => the groupings are not equal
			return false;
		}
		if (v1.size()==0){
			return false; //empty groupings are not equal (I know, it's a strange semantic, but seems fitting).
		}
		for (int i=0; i<v1.size(); i++){
			bool match_found = false;
			for (int j=0; j<v2.size(); j++){
				if (_equal(v1.at(i), v2.at(j))){
					match_found=true;					
				}
			}
			if (!match_found){//if a group on one side has no equivalent on the other side
				return false;
			}
		}
		return true;
	}

	void bll_PrintResults(int max_steps){
		int group_count=1;
		for (int i=1; i<=max_steps; i++){
			if (i!=1){
				cout<<"Groups of step: "<<i<<" (derived by adding 1 to groups in former step) "<<endl;
			}
			else{
				cout<<"Groups of step: "<<i<<endl;
			}
			for (int j=0; j<results.size(); j++){
				if (results.at(j).grp_step()==i){
					cout<<group_count<<" : ";
					vector<string> group = results.at(j).grp_groups();
					for (int k=0; k<group.size(); k++){
						cout<<group.at(k);
						if (k<group.size()-1){
							cout<<", ";
						}
					}
					cout<<endl;
					group_count++;
				}
			}
		}
	}

	void bll_BellCombination(vector<char>& columns)
	{
		int c = columns.size();
		if(c==1)
			{return;}
	
		for (int r=1; r<= num_steps; r++)
		{
			
			if (r==c){ // generating fatest part (this is just a short-cut)
				vector<string> p;
				string s="";
				for(int i=0; i< c; i++)
				{
					s+= columns.at(i);
				}
				p.push_back(s);
				results.push_back(grouping(p,r));
				
			}
			else if(r ==1) // generating thin part
			{	
				vector<string> p;
				for(int i=0; i< c; i++)
				{
					p.push_back(string(1, columns.at(i)));
				}
				results.push_back(grouping(p,r));
			}
			else if (r<c){//since we also do a check on initialization, we know that at this point r must be 2 or higher
				vector<grouping> groups_of_this_step;
				for (int i=0; i<results.size(); i++){
					if (results.at(i).grp_step()==r-1){//We only expand by one those groups from the former step
						for (int j=0; j<c; j++){
							string s=string(1, columns.at(j));
							bool can_be_expanded= false;
							int pos_to_erase=0;
							for (int k=0; k<results.at(i).grp_groups().size(); k++){
								if (results.at(i).grp_groups().at(k)==s){
									can_be_expanded=true;
									pos_to_erase=k;
									break;
								}
							}
							if (can_be_expanded){
								vector<string> expansion = results.at(i).grp_groups();
								expansion.erase(expansion.begin()+pos_to_erase);
								for (int l=0; l<expansion.size(); l++){
									vector<string> candidate= expansion;
									candidate[l]=candidate[l]+s;
									bool can_be_added = true;
									for (int m=0; m<groups_of_this_step.size(); m++){
										if(_equal_groupings(candidate, groups_of_this_step.at(m).grp_groups())){
											can_be_added=false;
											break;
										}
									}
									if (can_be_added){
										groups_of_this_step.push_back(grouping(candidate, r));
									}
								}						
							}
						}
					}
				}
				if (groups_of_this_step.size()>0){
					results.insert(results.end(), groups_of_this_step.begin(), groups_of_this_step.end());			
				}
			}

		}
	}
};

int main()
{
	// initializing
	char x;
	int columnno;
	int numsteps;
	set< char > inputcolumns;


	//input section
	cout<< "Number of columns:";
	do{
		cin>> columnno;
		if (columnno<0){
			cerr<<"The number of columns must be a positive integer"<<endl;
		}
	} while (columnno<0);


	//set values
	cout<<"Introduce distinct values... (chars only)"<<endl;
	while(inputcolumns.size()<columnno)
	{
		cin>> x;
		inputcolumns.insert(x);
	}
	
	//set number of steps
	cout<< "Number of steps:";
	do{
		cin>> numsteps;
		if (numsteps<0||numsteps>columnno){
			cerr<<"The number of steps must be a positive integer, and lesser or equal than the number of columns"<<endl;
		}
	} while (numsteps<0||numsteps>columnno);

      	bell bellobj(numsteps);
	vector <char> columns(inputcolumns.begin(), inputcolumns.end()); 
	
	//processing
	bellobj.bll_BellCombination(columns);

	// output section
	bellobj.bll_PrintResults(numsteps);

	return 0;
}
