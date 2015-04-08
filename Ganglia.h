#if !defined(AFX_NETWORK_H__8E7C932B_D833_4E1F_9EDC_ED09AFCF876A__INCLUDED_)
#define AFX_NETWORK_H__8E7C932B_D833_4E1F_9EDC_ED09AFCF876A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>

/*
 * Reads and writes Ganglia configuration files.
 */
class Ganglia {

public:
	Ganglia();
//	Gangla(std::string file_name);
	
	int getNumInputs();
	void setNumInputs(int num_inputs);

	int getNumOutputs();
	void setNumOutputs(int num_outputs);

	float** getNetworkWeights();
	void setNetworkWeights(float** network_weights);

	// TODO Add functions here which simplify updating network weights,
	// e.g. replaceAll(oldWeight, newWeight) or setInterneuronWeights(weight)

	void setFileName(std::string file_name);

	/*
	 * Write the ganglia file to disk.
	 */
	void writeFile();

private:
	std::string file_name;

	int num_inputs;
	int num_outputs;
}

#endif // !defined(AFX_NETWORK_H__8E7C932B_D833_4E1F_9EDC_ED09AFCF876A__INCLUDED_)
