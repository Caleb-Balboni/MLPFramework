#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>
using namespace std;
class Network
{
public:
	vector<vector<double>> inputs = 
	{
	{1,0},
	{1,1},
	{1,2},
	{1,3},
	{1,4},
	{1,5},
	{1,6},
	{1,7},
	{1,8},
	{1,9}
	};
	double outputs[10]{ 0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9, 1 };

	bool changeinputs(vector<vector<double>> inp)
	{
		inputs.clear();
		for (int i = 0; i < inp.size(); i++)
		{
			inputs.push_back(inp[i]);
		}
		if ((inputs[0][0] == 1) && (inputs[0][1] == 5))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	double random_num(double start, double end)
	{
		std::random_device rd;
		std::default_random_engine generator(rd()); // rd() provides a random seed
		std::uniform_real_distribution<double> distribution(start, end);
		double random = distribution(generator);
		cout << "weight: " << random << endl;
		return random;
	}

	double calcderivitive(double output)
	{
		return output * (1.0 - output);
	}
	double sigmoid(double x)
	{
		double result;
		result = 1 / (1 + exp(-x));
		return result;
	}
	struct Neuron
	{
		vector <double> Weights;
		double output = 0;
		double error = 0;
	};
	struct Layer
	{
		vector <Neuron> Neurons;
		double Bias = 1;
		double BiasWeight;
		Layer()
		{
			Network net;
			cout << "Bias ";
			BiasWeight = net.random_num(-1.75, 1.75);
		}
	};
	Neuron CreateNeuron(int WA)
	{
		Neuron TempN;
		for (int i = 0; i < WA; i++)
		{
			TempN.Weights.push_back(random_num(-1.75, 1.75));
		}
		TempN.output = 0;
		TempN.error = 0;
		return TempN;
	}
	vector <Layer> Layers;
	void CreateLayer(int NeuronN)
	{
		Layer templayer;
		for (int i = 0; i < NeuronN; i++)
		{
			if (Layers.size() == 0)
				templayer.Neurons.push_back(CreateNeuron(inputs[0].size()));
			else
				templayer.Neurons.push_back(CreateNeuron(Layers[Layers.size() - 1].Neurons.size()));
		}
		Layers.push_back(templayer);
	}
	void Backpropegate(int z)
	{
		for (int i = Layers.size() - 1; i >= 0; i--)
		{
			if (i == Layers.size() - 1)
			{
				Layers[i].Neurons[0].error = ((outputs[z] - Layers[i].Neurons[0].output) * calcderivitive(Layers[i].Neurons[0].output));
				for (int x = 0; x < Layers[i].Neurons[0].Weights.size(); x++)
					Layers[i].Neurons[0].Weights[x] += Layers[i].Neurons[0].error * Layers[i - 1].Neurons[x].output;

				Layers[i].BiasWeight += Layers[i].Bias * Layers[i].Neurons[0].error;
			}
			else
			{
				for (int x = 0; x < Layers[i].Neurons.size(); x++)
				{
					for (int y = 0; y < Layers[i + 1].Neurons.size(); y++)
					{
						double temperror = 0;
						for (int f = 0; f < Layers[i + 1].Neurons[y].Weights.size(); f++)
							temperror += Layers[i + 1].Neurons[y].error * Layers[i + 1].Neurons[y].Weights[f];

						Layers[i].Neurons[x].error = temperror * calcderivitive(Layers[i].Neurons[x].output);
						//cout << "dbg error values: " << Layers[i].Neurons[x].error << endl;
					}
					for (int f = 0; f < Layers[i].Neurons[x].Weights.size(); f++)
					{
						if (i == 0)
						{
							Layers[i].Neurons[x].Weights[f] += Layers[i].Neurons[x].error * inputs[z][f];
						}
						else
						{
							Layers[i].Neurons[x].Weights[f] += Layers[i].Neurons[x].error * Layers[i - 1].Neurons[x].output;
						}
					}
					Layers[i].BiasWeight += Layers[i].Bias * Layers[i].Neurons[x].error;
				}

			}
		}


	}
	void ReturnWeights()
	{
		for (int i = 0; i < Layers.size(); i++)
		{
			cout << "Layer: " << i << endl;
			for (int x = 0; x < Layers[i].Neurons.size(); x++)
			{
				for (int y = 0; y < Layers[i].Neurons[x].Weights.size(); y++)
				{
					if (y == 0)
						cout << Layers[i].Neurons[x].Weights[y] << "x" << " + ";
					if (y == 1)
						cout << Layers[i].Neurons[x].Weights[y] << "y" << " + ";
				}
				cout << Layers[i].BiasWeight << " = 0";
				cout << endl;
			}
		}
	}
	void TrainNetwork(int epochs, bool dbg) 
	{
		for (int z = 0; z < epochs; z++)
		{
			for (int c = 0; c < inputs.size(); c++)
			{
				vector <double> Inputs = inputs[c];
				for (int i = 0; i < Layers.size(); i++)
				{
					for (int x = 0; x < Layers[i].Neurons.size(); x++)
					{
						double tempoutput = 0;
						for (int y = 0; y < Inputs.size(); y++)
							tempoutput += Inputs[y] * Layers[i].Neurons[x].Weights[y];
						Layers[i].Neurons[x].output = sigmoid(tempoutput + Layers[i].Bias * Layers[i].BiasWeight);
					}
					Inputs.clear();
					for (int x = 0; x < Layers[i].Neurons.size(); x++)
					{
						Inputs.push_back(Layers[i].Neurons[x].output);
					}
				}
				if (dbg)
					cout << "output: " << Inputs[0] << endl;

				Backpropegate(c);
			}

		}
	}
	void outputnetwork(int epochs,bool dbg)
	{
		for (int z = 0; z < epochs; z++)
		{
			for (int c = 0; c < inputs.size(); c++)
			{
				vector <double> Inputs = inputs[c];
				for (int i = 0; i < Layers.size(); i++)
				{
					for (int x = 0; x < Layers[i].Neurons.size(); x++)
					{
						double tempoutput = 0;
						for (int y = 0; y < Inputs.size(); y++)
							tempoutput += Inputs[y] * Layers[i].Neurons[x].Weights[y];
						Layers[i].Neurons[x].output = sigmoid(tempoutput + Layers[i].Bias * Layers[i].BiasWeight);
					}
					Inputs.clear();
					for (int x = 0; x < Layers[i].Neurons.size(); x++)
					{
						Inputs.push_back(Layers[i].Neurons[x].output);
					}
				}
				if (dbg)
					cout << "output: " << Inputs[0] << endl;

			
			}

		}
	}
};

int main()
{
	Network testnet;
	testnet.CreateLayer(4);
	testnet.CreateLayer(3);
	testnet.CreateLayer(1);
	testnet.TrainNetwork(100000,false);
	testnet.TrainNetwork(10, true);
	vector<vector<double>> inp =
	{
	{1,5}
	};
	cout << testnet.changeinputs(inp);
	testnet.outputnetwork(5, true);
	testnet.ReturnWeights();
}