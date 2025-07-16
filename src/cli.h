#include <vector>
#include <string>

int check_for_graph_file(const std::vector<std::string>& cliArguments, std::string& filename_graph);

int check_for_seed(const std::vector<std::string>& cliArguments, int& seed);

int check_for_options(const std::vector<std::string>& cliArguments, bool& print_td);