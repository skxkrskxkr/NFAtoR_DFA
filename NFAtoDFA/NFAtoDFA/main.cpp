#include "readnfa.h"


int main() {

	nfa t_nfa = nfa();
//	t_nfa.read("input_nfa.txt");  //reduced-nfa
//	t_nfa.read("input_nfa2.txt"); //e-nfa
	t_nfa.read("input_nfa3.txt");

	//std::cout << t_nfa.nfa_name << std::endl;

	
	/*for (int i = 0; i < t_nfa.state_length; i++) {
		std::cout <<t_nfa.state[i] << std::endl;
	}
	std::cout << std::endl;
	for (int i = 0; i < t_nfa.sigma_length; i++) {
		std::cout << t_nfa.sigma[i] << std::endl;
	}
	std::cout << t_nfa.start_state << std::endl;

	std::cout << std::endl;
	for (int i = 0; i < t_nfa.final_length; i++) {
		std::cout << t_nfa.final_state[i] << std::endl;
	}
	
	for (int i = 0; i < t_nfa.state_length * t_nfa.sigma_length; i++) {
		std::cout << t_nfa.func[i].input_state << ", " << t_nfa.func[i].input_sigma << " = " << t_nfa.func[i].output << std::endl;
	}*/

	
	t_nfa.to_dfa();
	std::cout << "nfa to dfa" << std::endl;
	t_nfa.print_table();
	t_nfa.reduce();

	return 0;
}