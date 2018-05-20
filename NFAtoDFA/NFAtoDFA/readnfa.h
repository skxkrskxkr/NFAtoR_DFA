#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

//class s_and_o {
//public:
//	std::string state;
//	std::string
//};
class func_table {
public:

	std::string input_state;
	std::string input_sigma;
	std::string output;
	
	func_table() {

	}
};

class dfa_table {
public:
	std::vector<std::string> state;
	std::vector<std::string> output;

	std::vector<std::string> new_state;
	std::vector<std::string> new_output;

	int *state_number;
	int *output_number;
};
class ar_s_o {
public:
	std::string state; int staten_num;
	std::string *output; int *output_num;
	int check = 0;

};
class nfa { //& is delta signal, @ is epcilone, # is nullset

public:
	std::string *state;	std::string *sigma; std::string *final_state;
	std::string start_state; char delta; std::string nfa_name;
	int state_num; int count; bool ep_check = false; int ep_check_num; int ep_length;
	int state_length; int sigma_length; int final_length;
	func_table *func; int func_table_index;
	dfa_table dfa; dfa_table ep_dfa;
	nfa(){
		dfa = dfa_table();
		ep_dfa = dfa_table();
	}

	void read(std::string txt) {
		std::ifstream in_nfa(txt);
		std::string get_string;
		if (!in_nfa.is_open()) {
			std::cout << "file open error" << std::endl;

			exit(1);
		}
		else {
			state_num = 0; count = 0;
			getline(in_nfa, get_string); // nfa state sepearte
			nfa_name = get_string.substr(0, get_string.find('='));

			for (int i = get_string.find_first_of('(') + 1; i < get_string.find_last_of(')'); i++) {
				if (get_string[i] == '{') {  
					int index_num = 0; count = count + 1;
					if (count == 1) { //state

						while (get_string[i + index_num] != '}') {  //state 개수파악
							if (get_string[i + index_num] == ',') {
								state_num++;
							}
							index_num++;
						}
						state = new std::string[state_num + 1]; // state array생성
						for (int i = 0; i < state_num + 1; i++) {
							state[i] = "";
							
						}
						state_num = 0; //state array index초기화
						i++;
						while (get_string[i] != '}') {
							if (get_string[i] != ',' && get_string[i] != ' ') {
								state[state_num].push_back(get_string[i]);
								i++;
							}
							else if (get_string[i] != ' ') { //{}내 , 인경우 //text파일내 띄어쓰기 주의
								i++;
							}
							else {
								state_num++;
								i++;
							}
						}
						state_length = state_num+1;
					}

					if (count == 2) { // sigma
						state_num = 0; //sigma 생성하기 위해 변수 사용
						while (get_string[i + index_num] != '}') {  //sigma 개수파악
							if (get_string[i + index_num] == ',') {
								state_num++;
							}
							index_num++;
						}
						sigma = new std::string[state_num + 1]; // state array생성
						for (int i = 0; i < state_num + 1; i++) {
							sigma[i] = "";
						}
						state_num = 0; //state array index초기화
						i++;
						while (get_string[i] != '}') {
							if (get_string[i] != ',' && get_string[i] != ' ') {
								sigma[state_num].push_back(get_string[i]);
								i++;
							}
							else if (get_string[i] != ' ') { // {}내 ,인 경우
								i++;
							}
							else {
								state_num++;
								i++;
							}
						}
						sigma_length = state_num + 1;
					}
					if (count == 3) { //end - state
						state_num = 0; //sigma 생성하기 위해 변수 사용
						while (get_string[i + index_num] != '}') {  //sigma 개수파악
							if (get_string[i + index_num] == ',') {
								state_num++;
							}
							index_num++;
						}
						final_state = new std::string[state_num + 1]; // state array생성
						for (int i = 0; i < state_num + 1; i++) {
							final_state[i] = "";
						}
						state_num = 0; //state array index초기화
						i++;
						while (get_string[i] != '}') {
							if (get_string[i] != ',' && get_string[i] != ' ') {
								final_state[state_num].push_back(get_string[i]);
								i++;
							}
							else if (get_string[i] != ' ') { // {}내 ,인 경우
								i++;
							}
							else {
								state_num++;
								i++;
							}
						}
						final_length = state_num + 1;
					}
				}
				else if (get_string[i] != ',' && get_string[i] != ' ' && get_string[i] != '&') { //start_state
					while (get_string[i] != ',') {
						if (get_string[i] != ',' && get_string[i] != ' ') {
							start_state.push_back(get_string[i]);
							i++;
						}
						else {
							i++;
						}
					}
			
				}
			}
			// state check

			for (int i = 0; i < state_length; i++) {
				if (state[i].compare(start_state) == 0) {
					break;
				}
				if (i == state_length-1) {
					std::cout << "nfa state내에 start state가 존재하지않습니다." << std::endl;
					exit(1);
				}
			}
			for (int a = 0; a < final_length; a++) {
				for (int b = 0; b < state_length; b++) {
					if (state[b].compare(final_state[a]) == 0) {
						break;
					}
					if (b == state_length - 1) {
						std::cout << "nfa state내에 final state가 존재하지않습니다." << std::endl;
						exit(1);
					}
				}
			}
			// delta function arrange
			
			func = new func_table[state_length * sigma_length];
			for (int i = 0; i < state_length * sigma_length; i++) { //reset
				func[i] = func_table();
			}
			func_table_index = 0; int input_mode = 0;
			int func_table_index2 = 0;
			while (getline(in_nfa, get_string)) { //delta function 입력받는곳

				if (get_string[0] == '&') {
					for (int i = get_string.find_first_of('(') + 1; i < get_string.find_last_of(')'); i++) {  //  input  = ouput 중에 input					
							if (get_string[i] != ',' && get_string[i] != ' ') {
								if (input_mode == 0) {
									func[func_table_index].input_state.push_back(get_string[i]);
								}
								else if (input_mode == 1) {
									func[func_table_index].input_sigma.push_back(get_string[i]);
								}
							}
							else if (get_string[i] != ' ') { //{}내 , 인경우
								;
							}
							else { //input state 끝, input sigma mode로 변환
								//func_table_index++;
								if (input_mode == 0) input_mode = 1;
							}
							if (i + 1 == get_string.find_last_of(')')) {
								input_mode = 0; func_table_index++;
							}
					}
					for (int i = get_string.find_first_of('{') + 1; i < get_string.find_last_of('}'); i++) {  //  input  = ouput 중에 input					
						if (get_string[i] == ',') {
							func[func_table_index2].output.push_back(' ');
						}
						else if (get_string[i] != ' ') {
								func[func_table_index2].output.push_back(get_string[i]);
						}
						else { ; }
						if (i + 1 == get_string.find_last_of('}')) {
							func_table_index2++;
						}
					}
				}
				
				else if (get_string[0] == '*') {
					ep_check = true; std::string ep_temp = "";
					for (int i = get_string.find_first_of('(') + 1; i < get_string.find_last_of(')'); i++) {  //  input  = ouput 중에 input					
						if (get_string[i] != ',' && get_string[i] != ' ') {
							ep_temp.push_back(get_string[i]);
						}
						else if (get_string[i] != ' ') {
							ep_dfa.state.push_back(ep_temp);
							ep_temp = "";
							break;
						}
						else { ; }
					}
					for (int i = get_string.find_first_of('{') + 1; i < get_string.find_last_of('}'); i++) {  //  input  = ouput 중에 output					
						if (get_string[i] != ',' && get_string[i] != ' ') {
							ep_temp.push_back(get_string[i]);
						}
						else if (get_string[i] != ' ') {
							ep_temp.push_back(' ');
						}
						else { 
							;
						}
					}
					ep_dfa.output.push_back(ep_temp); ep_temp = "";

				}
				else { //txt file error
					std::cout << "input file error in function" << std::endl;
					exit(1);
				}
			}
		}




		in_nfa.close();
	}
	void make_new_state(std::string output, std::string &st_temp2) {  //재귀함수 갈수있는 모든 state를 들러서 state를 합치는과정
		std::string st_temp; bool infinite_check = false;
			std::stringstream stream(output);
			while (stream >> st_temp) {
				if (st_temp2.find(st_temp) == std::string::npos)
					st_temp2 = st_temp2 + st_temp;
				else {
					infinite_check = true;
				}
				if (!infinite_check) {
					for (int i = 0; i < ep_dfa.state.size(); i++) {
						if (ep_dfa.state[i] == st_temp) {
							make_new_state(ep_dfa.output[i], st_temp2);
						}
					}
				}
			}
	}
	void to_dfa() {
		//if() 입실론 nfa인경우
		if (ep_check) {
			//for (int i = 0; i < ep_dfa.state.size(); i++) {
			//	std::cout << ep_dfa.state[i] << std::endl;
			//}
			//for (int i = 0; i < ep_dfa.output.size(); i++) {
			//
			//	std::cout << ep_dfa.output[i] << std::endl;
			//}

				std::string st_temp;  std::string st_temp2;
				st_temp2 = ep_dfa.state[0];
				std::stringstream stream(ep_dfa.output[0]);
				while (stream >> st_temp) {
						if (st_temp2.find(st_temp) == std::string::npos)
								st_temp2 = st_temp2 + st_temp;

						for (int i = 0; i < ep_dfa.state.size(); i++) {
							if (ep_dfa.state[i] == st_temp) {
								make_new_state(ep_dfa.output[i], st_temp2);
	
							}
						}
				}
				//for (int i = 0; i < ep_dfa.state.size(); i++) {
				//	if (st_temp2.find(ep_dfa.state[i]) != std::string::npos) {				
				//		make_new_state(ep_dfa.state[i], st_temp2);	
				//	}					
				//}
				ep_dfa.new_state.push_back(st_temp2);

				int b = 0;
				while (true) {
					std::string temp; std::string temp2 = ""; bool state_non_exist = true;
					for (int w = 0; w < sigma_length; w++) {
						temp2 = ""; state_non_exist = true;
						for (int i = 0; i < state_length * sigma_length; i++) {
							if (ep_dfa.new_state[b].find(func[i].input_state) != std::string::npos) {
								if (func[i].input_sigma == sigma[w]) {
									std::stringstream stream(func[i].output);
									while (stream >> temp) {
					//				std::cout << ep_dfa.new_state[b] <<" vs " << func[i].input_state <<" i = " <<i << " w= "<<w << " " << "temp = " << temp << std::endl;
										if (temp != "#") {
											if (temp2.find(temp) == std::string::npos)
												temp2 = temp2 + temp;
										}
									}
								}
							}
						}
						//	std::cout << "temp2 =" << temp2 << std::endl;
						for (int i = 0; i < ep_dfa.state.size(); i++) {		
							if (temp2.find(ep_dfa.state[i]) != std::string::npos) {
			//					std::cout << temp2.find(ep_dfa.state[i]) << " temp2 = " << st_temp2 << " vs " << ep_dfa.state[i] << std::endl;
								make_new_state(ep_dfa.output[i], temp2);
		//						std::cout << "temp2 = " << temp2 << std::endl;				
							}
						}
						for (int p = 0; p < ep_dfa.new_state.size(); p++) {
							if (ep_dfa.new_state[p].compare(temp2) == 0) {
								//	std::cout << dfa.state[p] << " vs " << temp2 << std::endl;
								state_non_exist = false;
								break;
							}
						}
						if (state_non_exist) { /*std::cout << "temp2 input 성공" << std::endl;*/  if (temp2 != "") ep_dfa.new_state.push_back(temp2); }
						//	std::cout << "output"<< std::endl;
						if (temp2 == "" || temp2 == " ") {
							ep_dfa.new_output.push_back("#");
						}
						else {
				//			std::cout << "temp2 output으로" << temp2 << std::endl;
							ep_dfa.new_output.push_back(temp2);
						}
						if (w == sigma_length - 1) {
							b++;
						}
					}

					if (b == ep_dfa.new_state.size()) {
					//	std::cout << "사이즈는 " << b << std::endl;
						break;
					}

				}
				/*for (int i = 0; i < ep_dfa.new_state.size(); i++) {
					std::cout << "state" << std::endl;
					std::cout << ep_dfa.new_state[i] << std::endl;
				}
				for (int i = 0; i < ep_dfa.new_output.size(); i++) {
					std::cout << "output" << std::endl;
					std::cout << ep_dfa.new_output[i] << std::endl;
				}*/
		}

		//else 그냥 nfa인 경우
		else {
			dfa.state.push_back(start_state);

			//
			for (int a = 0; a < state_length * sigma_length; a++) {
				if (func[a].input_state == start_state) {  //dfa start
					std::stringstream stream(func[a].output);
					std::string temp; std::string temp2 = "";
					bool ck = true;
					while (stream >> temp) {
						if (temp != "#") {
							temp2 = temp2 + temp;

						}
						else {
							temp2 = "#";
						}
					}
					if (temp2 != "#") {
						for (int p = 0; p < dfa.state.size(); p++) {
							if (dfa.state[p].compare(temp2) == 0) {
								ck = false;
								break;
							}
						}
						if (ck) dfa.state.push_back(temp2);
					}
					dfa.output.push_back(temp2);
				}
			}
			int b = 1;
			while (true) { //복합적인 state의 결과값을 구하는 과정
				std::string temp; std::string temp2 = ""; bool state_non_exist = true;
				for (int w = 0; w < sigma_length; w++) {
					temp2 = ""; state_non_exist = true;
					for (int i = 0; i < state_length * sigma_length; i++) {
						if (dfa.state[b].find(func[i].input_state) != std::string::npos) {
							if (func[i].input_sigma == sigma[w]) {
								std::stringstream stream(func[i].output);
								while (stream >> temp) {
									//	std::cout << dfa.state[b] <<" vs " << func[i].input_state <<" i = " <<i << " w= "<<w << " " << "temp = " << temp << std::endl;
									if (temp != "#") {
										if (temp2.find(temp) == std::string::npos)
											temp2 = temp2 + temp;
									}
								}
							}
						}
					}
					//	std::cout << "temp2 =" << temp2 << std::endl;

					for (int p = 0; p < dfa.state.size(); p++) {
						if (dfa.state[p].compare(temp2) == 0) {
							//	std::cout << dfa.state[p] << " vs " << temp2 << std::endl;
							state_non_exist = false;
							break;
						}
					}
					if (state_non_exist) { /*std::cout << "temp2 input 성공" << std::endl;*/  if (temp2 != "") dfa.state.push_back(temp2); }
					//	std::cout << "output"<< std::endl;
					if (temp2 == "" || temp2 == " ") {
						dfa.output.push_back("#");
					}
					else {
						dfa.output.push_back(temp2);
					}
					if (w == sigma_length - 1) {
						b++;
					}
				}

				if (b == dfa.state.size()) {
					//	std::cout << "사이즈는 " << b << std::endl;
					break;
				}

			}
			//for (int i = 0; i < dfa.state.size(); i++) {
			//	std::cout << i << std::endl;
			//	std::cout << dfa.state[i] << std::endl;
			//}
			//for (int i = 0; i < dfa.output.size(); i++) {
			//	std::cout << dfa.output[i] << std::endl;
			//}
		}
	}
	void reduce() {
		if (ep_check) {
			int max_num = 0; int null_check = 0;

			ar_s_o *dfa_arr = new ar_s_o[ep_dfa.new_state.size()];

			for (int i = 0; i < ep_dfa.new_state.size(); i++) {
				dfa_arr[i] = ar_s_o();
				dfa_arr[i].output = new std::string[sigma_length];
				dfa_arr[i].output_num = new int[sigma_length];

				dfa_arr[i].state = ep_dfa.new_state[i];
				dfa_arr[i].staten_num = 0;
				for (int k = 0; k < sigma_length; k++) {
					dfa_arr[i].output[k] = ep_dfa.new_output[i*sigma_length + k];
					dfa_arr[i].output_num[k] = 0;
					
				}
			}

			for (int i = 0; i < ep_dfa.new_state.size(); i++) {
				for (int k = 0; k < final_length; k++) {
					if (dfa_arr[i].state == final_state[k] || dfa_arr[i].state.find(final_state[k]) != std::string::npos)
					{
						dfa_arr[i].staten_num += 1;
						max_num++;
					}
					for (int p = 0; p < sigma_length; p++) {
						if (dfa_arr[i].output[p] == final_state[k] || dfa_arr[i].output[p].find(final_state[k]) != std::string::npos) {
							if (dfa_arr[i].output[p].find("#") == std::string::npos) {
								dfa_arr[i].output_num[p] += 1;
							}
						}
						if (dfa_arr[i].output[p] == "#") {
							dfa_arr[i].output_num[p] = -1;
							null_check = 1;
						}
					}
				}
			}
			if (null_check == 1) {
				max_num--;
			}
			//for (int i = 0; i < ep_dfa.new_state.size(); i++) {
			//	std::cout << dfa_arr[i].staten_num << "\t";
			//	for (int b = 0; b < sigma_length; b++) {
			//			std::cout << dfa_arr[i].output_num[b] << "\t";		
			//	}
			//	std::cout << std::endl;
			//}
			while (true) { //state의 수를 바꾸는 부분
				int temp;
				for (int m = 0; m < max_num + 1; m++) {
					std::vector<std::string> check_string;
					for (int i = 0; i < ep_dfa.new_state.size(); i++) {
						for (int p = i + 1; p < ep_dfa.new_state.size(); p++) {
							if (dfa_arr[i].staten_num == dfa_arr[p].staten_num && dfa_arr[p].staten_num == m) {
							//	std::cout << dfa_arr[i].state << " vs " << dfa_arr[p].state << std::endl;

								for (int k = 0; k < sigma_length; k++) {
									if (dfa_arr[i].output_num[k] != dfa_arr[p].output_num[k] && dfa_arr[i].output_num[k] > 0 && dfa_arr[p].output_num[k] > 0) { //output의 다를경우
						//				std::cout << dfa_arr[i].output_num[k] << " vs " << dfa_arr[p].output_num[k] << std::endl;
										dfa_arr[p].check = 1;
										check_string.push_back(dfa_arr[p].state);
 									}
									else if (dfa_arr[i].output_num[k] * dfa_arr[p].output_num[k] < 0 || dfa_arr[i].output_num[k] + dfa_arr[p].output_num[k] == -1) {
							//			std::cout << dfa_arr[i].output_num[k] << " vs " << dfa_arr[p].output_num[k] << std::endl;
										dfa_arr[p].check = 1;
										check_string.push_back(dfa_arr[p].state);
						//				std::cout << "비교대상" << dfa_arr[p].state << std::endl;
									}
								}
							}
							temp = max_num + 1;
				//			std::cout << "temp =" << temp << std::endl;

							for (int i = 0; i < ep_dfa.new_state.size(); i++) {
								for (int k = 0; k < check_string.size(); k++) {
									if (dfa_arr[i].state == check_string[k])
									{
							//			std::cout << "temp =" << temp << std::endl;
										dfa_arr[i].staten_num = temp;
										if (temp > max_num) {
											max_num++;
										}
									}
									for (int p = 0; p < sigma_length; p++) {
										if (dfa_arr[i].output[p] == check_string[k] && dfa_arr[i].output[p] != "#") {
											dfa_arr[i].output_num[p] = temp;
										}
									}
								}
							}
						}
					}
				}
				if (temp > max_num) {
					break;
				}
			}

			int number = 0;
			std::cout << std::endl;
			std::cout << "reduced dfa" << std::endl;
			std::cout << "&\t ";
			for (int i = 0; i < sigma_length; i++)
				std::cout << sigma[i] << "\t";
			std::cout << std::endl;
	
			for (int i = 0; i < ep_dfa.new_state.size(); i++) {
				bool same = false;
				for (int n = 0; n < i; n++) {
					if (dfa_arr[i].staten_num == dfa_arr[n].staten_num) {
						same = true;
					}
				}
				if (!same) {
					std::cout << dfa_arr[i].staten_num << "\t";
					for (int b = 0; b < sigma_length; b++) {
						if (dfa_arr[i].output[b] != "#") {
							std::cout << dfa_arr[i].output_num[b] << "\t";
						}
						else {
							std::cout << "#" << "\t";

						}
					}
					std::cout << std::endl;
				}
			}


		}
		else {
			int max_num=0;

			ar_s_o *dfa_arr = new ar_s_o[dfa.state.size()];

			for (int i = 0; i < dfa.state.size(); i++) {
				dfa_arr[i] = ar_s_o();
				dfa_arr[i].output = new std::string[sigma_length];
				dfa_arr[i].output_num = new int[sigma_length];

				dfa_arr[i].state = dfa.state[i];
				dfa_arr[i].staten_num = 0;
				for (int k = 0; k < sigma_length; k++) {
					dfa_arr[i].output[k] = dfa.output[i*sigma_length + k];
					dfa_arr[i].output_num[k] = 0;
				}
			}

			for (int i = 0; i < dfa.state.size(); i++) {
				for (int k = 0; k < final_length; k++) {
					if (dfa_arr[i].state == final_state[k] || dfa_arr[i].state.find(final_state[k]) != std::string::npos)
					{
						dfa_arr[i].staten_num += 1;
						max_num++;
					}
					for (int p = 0; p < sigma_length; p++) {
						if (dfa_arr[i].output[p] == final_state[k] || dfa_arr[i].output[p].find(final_state[k]) != std::string::npos) {
							dfa_arr[i].output_num[p] += 1;
						}
					}
				}
			}
			while (true) {
				int temp;
				for (int m = 0; m < max_num + 1; m++) {
					std::vector<std::string> check_string;
					for (int i = 0; i < dfa.state.size(); i++) {
						for (int p = i+1; p < dfa.state.size(); p++) {
							if (dfa_arr[i].staten_num == dfa_arr[p].staten_num && dfa_arr[p].staten_num == m) {
								for (int k = 0; k < sigma_length; k++) {
									if (dfa_arr[i].output_num[k] != dfa_arr[p].output_num[k]) { //output의 다를경우
										dfa_arr[p].check = 1;
										check_string.push_back(dfa_arr[p].state);
									}
								}
							}
							temp = max_num + 1;
							for (int i = 0; i < dfa.state.size(); i++) {
								for (int k = 0; k < check_string.size(); k++) {
									if (dfa_arr[i].state == check_string[k])
									{
							
										dfa_arr[i].staten_num = temp;
										if (temp > max_num) {
											max_num++;
										}
									}
									for (int p = 0; p < sigma_length; p++) {
										if (dfa_arr[i].output[p] == check_string[k]) {
											dfa_arr[i].output_num[p] = temp;
										}
									}
								}
							}
						}
					}
					//temp = max_num + 1;
					//for (int i = 0; i < dfa.state.size(); i++) {
					//	for (int k = 0; k < check_string.size(); k++) {
					//		if (dfa_arr[i].state == check_string[k])
					//		{
					//			std::cout << temp << std::endl;
					//			dfa_arr[i].staten_num = temp;
					//			if (temp > max_num) {
					//				max_num++;
					//			}
					//		}
					//		for (int p = 0; p < sigma_length; p++) {
					//			if (dfa_arr[i].output[p] == check_string[k]) {
					//				dfa_arr[i].output_num[p] = temp;
					//			}
					//		}
					//	}
					//}
				}
				if (temp > max_num) {
					break;
				}
			}
			int number = 0;
			std::cout << std::endl;
			std::cout << "reduced dfa" << std::endl;
			std::cout << "&\t ";
			for (int i = 0; i < sigma_length; i++)
				std::cout << sigma[i] << "\t";
			std::cout << std::endl;

			for (int i = 0; i < dfa.state.size(); i++) {
				bool same = false;
				for (int n = 0; n < i; n++) {
					if (dfa_arr[i].staten_num == dfa_arr[n].staten_num) {
						same = true;
					}
				}
				if (!same) {
					std::cout << dfa_arr[i].staten_num << "\t";
					for (int b = 0; b < sigma_length; b++) {
						std::cout << dfa_arr[i].output_num[b] << "\t";
					}
					std::cout << std::endl;
				}
			}
			

		}
	}
	void print_table() {
		if (ep_check) {
			std::cout << "&\t ";
			for (int i = 0; i < sigma_length; i++)
				std::cout << sigma[i] << "\t";
			std::cout << std::endl;

			for (int i = 0; i < ep_dfa.new_state.size(); i++) {
				std::cout << ep_dfa.new_state[i] << "\t";

				for (int b = i * sigma_length; b < ep_dfa.new_output.size(); b = b + sigma_length) {
					for (int c = 0; c < sigma_length; c++) {
						std::cout << ep_dfa.new_output[b + c] << "\t";
					}
					break;
				}
				std::cout << std::endl;
			}
		}
		else {
			std::cout << "&\t ";
			for (int i = 0; i < sigma_length; i++)
				std::cout << sigma[i] << "\t";
			std::cout << std::endl;

			for (int i = 0; i < dfa.state.size(); i++) {
				std::cout << dfa.state[i] << "\t";

				for (int b = i * sigma_length; b < dfa.output.size(); b = b + sigma_length) {
					for (int c = 0; c < sigma_length; c++) {
						std::cout << dfa.output[b + c] << "\t";
					}
					break;
				}
				std::cout << std::endl;
			}
		}
	}
	~nfa() {
		delete[] state;
		delete[] final_state;
		delete[] sigma;
	}
};