#include <fstream>      
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <math.h>


const float pi = 3.14159;
const int pixel = 256;

class Image
{
public:
	Image()
	{
		size = 0;
		buffer_size = 0;
		flag_H = 0;
		flag_S = 0;
		flag_I = 0;
		coefficient = 0;
		distrubution_I = new float[pixel];
		
		modified_distrubution_I = new float[pixel];
		probability_I  = new float[pixel];
		acc_probability_I = new float[pixel];
		acc_distribution_I = new float[pixel];
		HE_I = new float[pixel];

		distrubution_H = new float[360];
		modified_distrubution_H = new float[360];
		probability_H = new float[360];
		acc_probability_H = new float[360];
		acc_distribution_H = new float[360];
		HE_H = new float[360];;

		for (int i = 0; i < pixel; i++)
		{
			distrubution_I[i] = 0;
			modified_distrubution_I[i] = 0;
			probability_I[i] = 0;
			acc_probability_I[i] = 0;
			HE_I[i] = 0;
			acc_distribution_I[i] = 0;
		}
		for (int i = 0; i < 360; i++)
		{
			distrubution_H[i] = 0;
			modified_distrubution_H[i] = 0;
			probability_H[i] = 0;
			acc_probability_H[i] = 0;
			acc_distribution_H[i] = 0;
			HE_H[i] = 0;
		}

	}
	float min_3(float a, float b, float c);
	float min_2(float a, float b);
	float Max_2(float a, float b);
	float H_channel(float r, float g, float b);
	float S_channel(float r, float g, float b);
	float I_channel(float r, float g, float b);
	void testArea();
	void setSize(int s);
	void setBuffer(char* b);
	unsigned char* getBuffer_After();
	unsigned char* getOriginBuffer();
	void seperate_RGB();
	void ConvertToH();
	void ConvertToS();
	void ConvertToI();
	void I_HistogramEqulization();
	void S_GammaTransformation();
	void H_RangeExpand();
	void H_RangeExpand_R();
	void BackTo_RGB();
	void WriteBuffer();
	void setGammaCoefficient(float c);
	void seperate_RGB_Inter();
	void WriteBuffer_Inter();
	

private:
	int size;
	int buffer_size;
	unsigned char* buffer;
	unsigned char* buffer_R;
	unsigned char* buffer_G;
	unsigned char* buffer_B;
	float* buffer_H;
	float* buffer_S;
	float* buffer_I;
	unsigned int* buffer_after_R;
	unsigned int* buffer_after_G;
	unsigned int* buffer_after_B;

	float* distrubution_I;
	float* modified_distrubution_I;
	float* probability_I;
	float* acc_probability_I;
	float* acc_distribution_I;
	float* HE_I; 
	unsigned char* buffer_after;
	float* buffer_I_Modified;
	float* buffer_S_Modified;
	float* buffer_H_Modified;
	bool flag_H; // 0:not modified 1:modified
	bool flag_S;
	bool flag_I;
	float coefficient;

	float* distrubution_H;
	float* modified_distrubution_H;
	float* probability_H;
	float* acc_probability_H;
	float* acc_distribution_H;
	float* HE_H;

};

void Image::seperate_RGB_Inter()
{
	for (int i = 0; i < size; i++)
	{
		buffer_R[i] = buffer[3 * i + 0];
		buffer_G[i] = buffer[3 * i + 1];
		buffer_B[i] = buffer[3 * i + 2];
	}
}
void Image::WriteBuffer_Inter()
{
	for (int i = 0; i < size; i++)
	{
		buffer_after[3 * i + 0] = buffer_after_R[i];
		buffer_after[3 * i + 1] = buffer_after_G[i];
		buffer_after[3 * i + 2] = buffer_after_B[i];
	}
}

void Image::H_RangeExpand_R()
{
	for (int i = 0; i < size; i++)
	{
		distrubution_H[(int)round(buffer_H[i])]++; //compute the amount of each grey level
	}
	float total_distrubution = 0;
	float total_probability = 0;
	float Total = 0;
	for (int i = 0; i < 180; i++) //compute total distrubution of red
	{
		Total = Total + distrubution_H[i];
		//std::cout << distrubution_H[i] << std::endl;
	}

	for (int i = 0; i < 180; i++) //compute distribution probability and accumulative probability
	{
		total_distrubution = total_distrubution + distrubution_H[i];
		acc_distribution_H[i] = total_distrubution;
		probability_H[i] = distrubution_H[i] / Total;
		total_probability = total_probability + probability_H[i];
		acc_probability_H[i] = total_probability;
	}

	for (int i = 0; i < 180; i++) //Histogram Eqaulization
	{
		HE_I[i] = (int)((acc_distribution_H[i]) / (Total) * 180);
		std::cout << HE_I[i] << std::endl;
	}



	int temp;
	for (int i = 0; i < size; i++)
	{
		temp = (int)(round(buffer_H[i]));
		if (temp < 180 && temp >= 0)
		{
			buffer_H_Modified[i] = HE_I[temp];
		}
		else
		{
			buffer_H_Modified[i] = buffer_H[i];
		}
	}
	flag_H = 1;
}

void Image::H_RangeExpand()
{
	for (int i = 0; i < size; i++)
	{
		distrubution_H[(int)round(buffer_H[i])]++; //compute the amount of each grey level
	}
	float total_distrubution = 0;
	float total_probability = 0;

	for (int i = 0; i < 360; i++) //compute distribution probability and accumulative probability
	{
		total_distrubution = total_distrubution + distrubution_H[i];
		acc_distribution_H[i] = total_distrubution;
		probability_H[i] = distrubution_H[i] / size;
		total_probability = total_probability + probability_H[i];
		acc_probability_H[i] = total_probability;
	}

	for (int i = 0; i < 360; i++) //Histogram Eqaulization
	{
		HE_I[i] = (int)((acc_distribution_H[i]) / (size) * 360);
	}

	int temp;
	for (int i = 0; i < size; i++)
	{
		temp = (int)(round(buffer_H[i]));
		buffer_H_Modified[i] = HE_I[temp];
	}
	flag_H = 1;
}


void Image::setGammaCoefficient(float c)
{
	coefficient = c;
}
void Image::setBuffer(char* b)
{
	buffer = new unsigned char[buffer_size];
	buffer_after = new unsigned char[buffer_size];
	buffer_R = new unsigned char[size];
	buffer_G = new unsigned char[size];
	buffer_B = new unsigned char[size];
	buffer_H = new float[size];
	buffer_S = new float[size];
	buffer_I = new float[size];
	buffer_after_R = new unsigned int[size];
	buffer_after_G = new unsigned int[size];
	buffer_after_B = new unsigned int[size];
	buffer_I_Modified = new float[size];
	buffer_S_Modified = new float[size];
	buffer_H_Modified = new float[size];
	for (int i = 0; i < size; i++)
	{
		buffer_R[i] = 0;
		buffer_G[i] = 0;
		buffer_B[i] = 0;
		buffer_H[i] = 0;
		buffer_S[i] = 0;
		buffer_I[i] = 0;
		buffer_after_R[i] = 0;
		buffer_after_G[i] = 0;
		buffer_after_B[i] = 0;
		buffer_I_Modified[i] = 0;
		buffer_S_Modified[i] = 0;
		buffer_H_Modified[i] = 0;
	}
	for (int i = 0; i < buffer_size; i++)
	{
		buffer[i] = 0;
		buffer_after[i] = 0;
	}
	buffer = (unsigned char*)b;

}

void Image::setSize(int s)
{
	buffer_size = s;
	size = buffer_size / 3;
}

unsigned char* Image::getOriginBuffer()
{
	return buffer;
}

unsigned char* Image::getBuffer_After()
{
	return buffer_after;
}

void Image::WriteBuffer()
{
	for (int i = 0; i < size; i++)
	{
		buffer_after[i] = buffer_after_R[i];
		buffer_after[i + size] = buffer_after_G[i];
		buffer_after[i + size * 2] = buffer_after_B[i];
	}
}

void Image::BackTo_RGB()
{
	float h, s, ii;
	for (int i = 0; i < size; i++)
	{
		if (flag_H == 1)
		{
			h = buffer_H_Modified[i];
		}
		else
		{
			h = buffer_H[i];
		}
		if (flag_S == 1)
		{
			s = buffer_S_Modified[i];
		}
		else
		{
			s = buffer_S[i];
		}
		if (flag_I == 1)
		{
			ii = buffer_I_Modified[i];
		}
		else
		{
			ii = buffer_I[i];
		}

		if (0 <= h && h < 120)
		{
			h = h;
			buffer_after_R[i] = min_2(255, ii * (1 + s * cos(h / 180 * pi) / cos((60 - h) / 180 * pi)));
			buffer_after_B[i] = min_2(255, ii * (1 - s));
			buffer_after_G[i] = min_2(255, 3 * ii - buffer_after_R[i] - buffer_after_B[i]);
		}
		else if (120 <= h && h < 240)
		{
			h = h - 120;
			buffer_after_R[i] = min_2(255, ii * (1 - s));
			buffer_after_G[i] = min_2(255, ii * (1 + s * cos(h / 180 * pi) / cos((60 - h) / 180 * pi)));
			buffer_after_B[i] = min_2(255, 3 * ii - (buffer_after_R[i] + buffer_after_G[i]));
			
		}
		else if (240 <= h && h < 360)
		{
			h = h - 240;
			buffer_after_G[i] = min_2(255, ii * (1 - s));
			buffer_after_B[i] = min_2(255, ii * (1 + s * cos(h / 180 * pi) / cos((60 - h) / 180 * pi)));
			buffer_after_R[i] = min_2(255, 3 * ii - (buffer_after_G[i] + buffer_after_B[i]));	
		}
	}
}


float Image::Max_2(float a, float b)
{
	float m = INT_MIN;
	if (a > m)
	{
		m = a;
	}
	if (b > m)
	{
		m = b;
	}
	return m;
}

void Image::S_GammaTransformation()
{
	for (int i = 0; i < size; i++)
	{
		buffer_S_Modified[i] = pow(buffer_S[i], coefficient);
	}
	flag_S = 1;
}

void Image::I_HistogramEqulization()
{
	
	for (int i = 0; i < size; i++)
	{
		distrubution_I[(int)round(buffer_I[i])]++; //compute the amount of each grey level
	}
	float total_distrubution = 0;
	float total_probability = 0;

	for (int i = 0; i < pixel; i++) //compute distribution probability and accumulative probability
	{
		total_distrubution = total_distrubution + distrubution_I[i];
		acc_distribution_I[i] = total_distrubution;
		probability_I[i] = distrubution_I[i] / size; 
		total_probability = total_probability + probability_I[i];
		acc_probability_I[i] = total_probability;
	}

	for (int i = 0; i < pixel; i++) //Histogram Eqaulization
	{
		HE_I[i] = (int)((acc_distribution_I[i]) / (size) * 255);
	}

	int temp;
	for (int i = 0; i < size; i++)
	{
		temp = (int)(round(buffer_I[i]));
		buffer_I_Modified[i] = HE_I[temp];
	}
	flag_I = 1;
}

void Image::ConvertToH()
{
	for (int i = 0; i < size; i++)
	{
		buffer_H[i] = H_channel((float)buffer_R[i], (float)buffer_G[i], (float)buffer_B[i]);
	}
}

void Image::ConvertToS()
{
	for (int i = 0; i < size; i++)
	{
		buffer_S[i] = S_channel((float)buffer_R[i], (float)buffer_G[i], (float)buffer_B[i]);
	}
}
void Image::ConvertToI()
{
	for (int i = 0; i < size; i++)
	{
		buffer_I[i] = I_channel((float)buffer_R[i], (float)buffer_G[i], (float)buffer_B[i]);
	}
}


void Image::seperate_RGB()
{
	for (int i = 0; i < size; i++)
	{
		buffer_R[i] = buffer[i];
		buffer_G[i] = buffer[size + i];
		buffer_B[i] = buffer[size * 2 + i];
	}
	
}
float Image::min_3(float a, float b, float c)
{
	float m = INT_MAX;
	if (a < m)
	{
		m = a;
	}
	if (b < m)
	{
		m = b;
	}
	if (c < m)
	{
		m = c;
	}

	return m;
}

float Image::min_2(float a, float b)
{
	float m = INT_MAX;
	if (a < m)
	{
		m = a;
	}
	if (b < m)
	{
		m = b;
	}

	return m;
}

float Image::H_channel(float r, float g, float b)
{
	float R_minus_G = r - g;
	float R_minus_B = r - b;
	float G_minus_B = g - b;
	float h = acos(((1.0 / 2.0) * (R_minus_G + R_minus_B)) / sqrt(R_minus_G * R_minus_G + (R_minus_B * G_minus_B)))*360.0/(2*pi);
	if (b > g)
		h = 360 - h;
	return h;
}

float Image::S_channel(float r, float g, float b)
{
	float s = 1 - (3 / (r + g + b) * min_3(r, g, b));
	return s;
}

float Image::I_channel(float r, float g, float b)
{
	float i = (r + g + b) / 3;
	return i;
}

void Image::testArea()
{
	float r, g, b;
	r = 0.6;
	g = 0.5;
	b = 0.4;
	std::cout << "Test Area" << std::endl;
	std::cout << "R = " << r << std::endl;
	std::cout << "G = " << g << std::endl;
	std::cout << "B = " << b << std::endl;
	float ii, s, h;
	h = H_channel(r, g, b);
	s = S_channel(r, g, b);
	ii = I_channel(r, g, b);
	std::cout << "H_channel:" << h << std::endl;
	std::cout << "S_channel:" << s << std::endl;
	std::cout << "I_channel:" << ii << std::endl;
	float r2, g2, b2;
		if (0 <= h && h < 120)
		{
			h = h;
			r2 = ii * (1 + (s * cos(h) / (cos(60 - h))));
			g2 = 3 * ii - (r + b);
			b2 = ii * (1 - s);
		}
		else if (120 <= h && h < 240)
		{
			h = h - 120;
			r2 = ii * (1 - s);
			g2 = ii * (1 + (s * cos(h) / (cos(60 - h))));
			b2 = 3 * ii - (r + g);
		}

		else if (240 <= h && h < 360)
		{
			h = h - 240;
			r2 = 3 * ii - (g + b);
			g2 = ii * (1 - s);
			b2 = ii * (1 + (s * cos(h) / (cos(60 - h))));
		}

	std::cout << "r" << r2 << std::endl;
	std::cout << "g" << g2 << std::endl;
	std::cout << "b" << b2 << std::endl;
	std::cout << "-------------------" << std::endl;
}

int main() {

	Image baboon_I, lena_I;
	Image baboon_S, lena_S;
	Image baboon_H, lena_H;
	Image baboon_Mix, lena_Mix;
	Image peppers;
	Image peppers_I;
	Image peppers_S;
	Image peppers_H;
	Image peppers_Mix;

	//Read File

	//peppers
	//peppers.raw is interleave
	std::ifstream infile_peppers("peppers.raw", std::ifstream::binary);
	// get size of file
	infile_peppers.seekg(0, infile_peppers.end);
	int size_peppers = infile_peppers.tellg();
	infile_peppers.seekg(0);
	// allocate memory for file content
	char* buffer_peppers = new  char[size_peppers];

	// read content of infile
	infile_peppers.read(buffer_peppers, size_peppers);
	infile_peppers.close();

	peppers_I.setSize(size_peppers);
	peppers_I.setBuffer(buffer_peppers);
	peppers_S.setSize(size_peppers);
	peppers_S.setBuffer(buffer_peppers);
	peppers_H.setSize(size_peppers);
	peppers_H.setBuffer(buffer_peppers);
	peppers_Mix.setSize(size_peppers);
	peppers_Mix.setBuffer(buffer_peppers);

	//baboon
	std::ifstream infile_baboon("baboon.raw", std::ifstream::binary);
	// get size of file
	infile_baboon.seekg(0, infile_baboon.end);
	int size_baboon = infile_baboon.tellg();
	infile_baboon.seekg(0);
	// allocate memory for file content
	char* buffer_baboon = new  char[size_baboon];

	// read content of infile
	infile_baboon.read(buffer_baboon, size_baboon);
	infile_baboon.close();

	baboon_I.setSize(size_baboon);
	baboon_I.setBuffer(buffer_baboon);
	baboon_S.setSize(size_baboon);
	baboon_S.setBuffer(buffer_baboon);
	baboon_H.setSize(size_baboon);
	baboon_H.setBuffer(buffer_baboon);
	baboon_Mix.setSize(size_baboon);
	baboon_Mix.setBuffer(buffer_baboon);
	
	//lena
	std::ifstream infile_lena("lena.raw", std::ifstream::binary);
	// get size of file
	infile_lena.seekg(0, infile_lena.end);
	long size_lena = infile_lena.tellg();
	infile_lena.seekg(0);
	
	// allocate memory for file content
	char* buffer_lena = new  char[size_lena];

	// read content of infile
	infile_lena.read(buffer_lena, size_lena);
	infile_lena.close();

	lena_I.setSize(size_lena);
	lena_I.setBuffer(buffer_lena);
	lena_S.setSize(size_lena);
	lena_S.setBuffer(buffer_lena);
	lena_H.setSize(size_lena);
	lena_H.setBuffer(buffer_lena);
	lena_Mix.setSize(size_lena);
	lena_Mix.setBuffer(buffer_lena);
	
	// I Channel histogram equalization
	baboon_I.seperate_RGB();
	baboon_I.ConvertToH();
	baboon_I.ConvertToS();
	baboon_I.ConvertToI();
	baboon_I.I_HistogramEqulization();
	baboon_I.BackTo_RGB();
	baboon_I.WriteBuffer();
	
	lena_I.seperate_RGB();
	lena_I.ConvertToH();
	lena_I.ConvertToS();
	lena_I.ConvertToI();
	lena_I.I_HistogramEqulization();
	lena_I.BackTo_RGB();
	lena_I.WriteBuffer();

	peppers_I.seperate_RGB_Inter();
	peppers_I.ConvertToH();
	peppers_I.ConvertToS();
	peppers_I.ConvertToI();
	peppers_I.I_HistogramEqulization();
	peppers_I.BackTo_RGB();
	peppers_I.WriteBuffer_Inter();

	std::ofstream outfile_baboon_result_I("baboon_result_I.raw", std::ofstream::binary);
	std::ofstream outfile_lena_result_I("lena_result_I.raw", std::ofstream::binary);
	std::ofstream outfile_peppers_I("peppers_result_I.raw", std::ofstream::binary);

	// write to outfile
	outfile_baboon_result_I.write((char*)baboon_I.getBuffer_After(), size_baboon);
	outfile_lena_result_I.write((char*)lena_I.getBuffer_After(), size_lena);
	outfile_peppers_I.write((char*)peppers_I.getBuffer_After(), size_peppers);
	
	//close
	outfile_baboon_result_I.close();
	outfile_lena_result_I.close();
	outfile_peppers_I.close();

	// S Channel Gamma Transformation

	float coefficient;
	std::cout << "½Ð¿é¤JGamma Transformation«Y¼Æ:" << std::endl;;
	std::cin >> coefficient;
	std::cout << "---" << std::endl;
	
	baboon_S.seperate_RGB();
	baboon_S.ConvertToH();
	baboon_S.ConvertToS();
	baboon_S.ConvertToI();
	baboon_S.setGammaCoefficient(coefficient);
	baboon_S.S_GammaTransformation();
	baboon_S.BackTo_RGB();
	baboon_S.WriteBuffer();

	lena_S.seperate_RGB();
	lena_S.ConvertToH();
	lena_S.ConvertToS();
	lena_S.ConvertToI();
	lena_S.setGammaCoefficient(coefficient);
	lena_S.S_GammaTransformation();
	lena_S.BackTo_RGB();
	lena_S.WriteBuffer();

	peppers_S.seperate_RGB_Inter();
	peppers_S.ConvertToH();
	peppers_S.ConvertToS();
	peppers_S.ConvertToI();
	peppers_S.setGammaCoefficient(coefficient);
	peppers_S.S_GammaTransformation();
	peppers_S.BackTo_RGB();
	peppers_S.WriteBuffer_Inter();

	std::ofstream outfile_baboon_result_S("baboon_result_S.raw", std::ofstream::binary);
	std::ofstream outfile_lena_result_S("lena_result_S.raw", std::ofstream::binary);
	std::ofstream outfile_peppers_result_S("peppers_result_S.raw", std::ofstream::binary);

	// write to outfile
	outfile_baboon_result_S.write((char*)baboon_S.getBuffer_After(), size_baboon);
	outfile_lena_result_S.write((char*)lena_S.getBuffer_After(), size_lena);
	outfile_peppers_result_S.write((char*)peppers_S.getBuffer_After(), size_peppers);

	//close
	outfile_baboon_result_S.close();
	outfile_lena_result_S.close();
	outfile_peppers_result_S.close();

	
	// H Channel Histogram equalization

	baboon_H.seperate_RGB();
	baboon_H.ConvertToH();
	baboon_H.ConvertToS();
	baboon_H.ConvertToI();
	baboon_H.H_RangeExpand();
	baboon_H.BackTo_RGB();
	baboon_H.WriteBuffer();

	lena_H.seperate_RGB();
	lena_H.ConvertToH();
	lena_H.ConvertToS();
	lena_H.ConvertToI();
	lena_H.H_RangeExpand();
	lena_H.BackTo_RGB();
	lena_H.WriteBuffer();

	peppers_H.seperate_RGB_Inter();
	peppers_H.ConvertToH();
	peppers_H.ConvertToS();
	peppers_H.ConvertToI();
	peppers_H.H_RangeExpand();
	peppers_H.BackTo_RGB();
	peppers_H.WriteBuffer_Inter();

	std::ofstream outfile_baboon_result_H("baboon_result_H.raw", std::ofstream::binary);
	std::ofstream outfile_lena_result_H("lena_result_H.raw", std::ofstream::binary);
	std::ofstream outfile_peppers_result_H("peppers_result_H.raw", std::ofstream::binary);

	// write to outfile
	outfile_baboon_result_H.write((char*)baboon_H.getBuffer_After(), size_baboon);
	outfile_lena_result_H.write((char*)lena_H.getBuffer_After(), size_lena);
	outfile_peppers_result_H.write((char*)peppers_H.getBuffer_After(), size_peppers);

	//close
	outfile_baboon_result_H.close();
	outfile_lena_result_H.close();
	outfile_peppers_result_H.close();

	// MIX

	baboon_Mix.seperate_RGB();
	baboon_Mix.ConvertToH();
	baboon_Mix.ConvertToS();
	baboon_Mix.ConvertToI();
	baboon_Mix.H_RangeExpand();
	baboon_Mix.setGammaCoefficient(coefficient);
	baboon_Mix.S_GammaTransformation();
	baboon_Mix.I_HistogramEqulization();
	baboon_Mix.BackTo_RGB();
	baboon_Mix.WriteBuffer();

	lena_Mix.seperate_RGB();
	lena_Mix.ConvertToH();
	lena_Mix.ConvertToS();
	lena_Mix.ConvertToI();
	lena_Mix.H_RangeExpand();
	lena_Mix.setGammaCoefficient(coefficient);
	lena_Mix.S_GammaTransformation();
	lena_Mix.I_HistogramEqulization();
	lena_Mix.BackTo_RGB();
	lena_Mix.WriteBuffer();

	peppers_Mix.seperate_RGB_Inter();
	peppers_Mix.ConvertToH();
	peppers_Mix.ConvertToS();
	peppers_Mix.ConvertToI();
	peppers_Mix.H_RangeExpand();
	peppers_Mix.setGammaCoefficient(coefficient);
	peppers_Mix.S_GammaTransformation();
	peppers_Mix.I_HistogramEqulization();
	peppers_Mix.BackTo_RGB();
	peppers_Mix.WriteBuffer_Inter();

	std::ofstream outfile_baboon_result_Mix("baboon_result_Mix.raw", std::ofstream::binary);
	std::ofstream outfile_lena_result_Mix("lena_result_Mix.raw", std::ofstream::binary);
	std::ofstream outfile_peppers_result_Mix("peppers_result_Mix.raw", std::ofstream::binary);

	// write to outfile
	outfile_baboon_result_Mix.write((char*)baboon_Mix.getBuffer_After(), size_baboon);
	outfile_lena_result_Mix.write((char*)lena_Mix.getBuffer_After(), size_lena);
	outfile_peppers_result_Mix.write((char*)peppers_Mix.getBuffer_After(), size_peppers);

	//close
	outfile_baboon_result_Mix.close();
	outfile_lena_result_Mix.close();
	outfile_peppers_result_Mix.close();


	// release dynamically-allocated memory
	delete[] buffer_baboon;
	delete[] buffer_lena;

	system("pause");
	return 0;
}