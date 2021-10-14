//Bernard Śniegowski

#include "Morfologia.h"
#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

class BitmapaExt : public Bitmapa {
public:
	unsigned x,y,z;
	bool* bitmapa;
	unsigned size;

	BitmapaExt(unsigned zakresX, unsigned zakresY, unsigned zakresZ) : x(zakresX), y(zakresY), z(zakresZ), size(x*y*z) {
		bitmapa = new bool[size];
		// for(unsigned int i=0; i<1; i++) bitmapa[i] = false;
		std::fill_n(bitmapa, size, false);
	}
	BitmapaExt(const BitmapaExt& bitmapa) : Bitmapa() {
		if(&bitmapa != NULL) {
			x = bitmapa.x;
			y = bitmapa.y;
			z = bitmapa.z;
			this->size = x*y*z;
			this->bitmapa = new bool[this->size];
			memcpy(((bool *) this->bitmapa), ((bool *) bitmapa.bitmapa), this->size);
		}
	}

	BitmapaExt& operator=(const BitmapaExt& bitmapa) {
		if(this != &bitmapa) {
			delete[] this->bitmapa;
			this->bitmapa = NULL;
			x = bitmapa.sx();
			y = bitmapa.sy();
			z = bitmapa.sz();
			size = x*y*z;
			this->bitmapa = new bool[x*y*z];
			memcpy(((bool *) this->bitmapa), ((bool *) bitmapa.bitmapa), size);
		}
		return *this;
	}

	unsigned sx() const { return x; }
	unsigned sy() const { return y; }
	unsigned sz() const { return z; }

	bool& operator()(unsigned i, unsigned j, unsigned k) { return bitmapa[x*y*k + x*j + i]; }
	bool operator()(unsigned i, unsigned j, unsigned k) const { return bitmapa[x*y*k + x*j + i]; }

	~BitmapaExt() {
		delete[] bitmapa;
	}

// private:
// 	unsigned x,y,z;
// 	bool* bitmapa;
// 	unsigned size;
};

ostream& operator<<(ostream& out, const Bitmapa& bitmapa) {
	unsigned x = bitmapa.sx();
	unsigned y = bitmapa.sy();
	unsigned z = bitmapa.sz();

	out << "{" << "\n";
	for(int i=0; i<x; i++) {
		out << " {" << "\n";
		for(int j=0; j<y; j++) {
			out << "  {";
			for(int k=0; k<z-1; k++) {
				out << bitmapa(i,j,k) << ",";
			}
			out << bitmapa(i,j,z-1) << "}";
			if(j == y-1) out << "\n";
			else out << "," << "\n";
		}
		if(i == x-1) out << " }" << "\n";
		else out << " }," << "\n";
	}
	out << "}";
	return out;
}


// class Inwersja : public Przeksztalcenie {
// public:
// 	Inwersja() {}
// 	~Inwersja() {}
// 	void przeksztalc(Bitmapa& bitmapa) {
// 		for(int i=0; i<bitmapa.sx(); i++) {
// 			for(int j=0; j<bitmapa.sy(); j++) {
// 				for(int k=0; k<bitmapa.sz(); k++) {
// 					bitmapa(i,j,k) = !bitmapa(i,j,k);
// 				}
// 			}
// 		}
// 	}
// };

// class Erozja : public Przeksztalcenie {
// public:
// 	Erozja() {}
// 	~Erozja() {}
// 	void przeksztalc(Bitmapa& bitmapa) {
// 		BitmapaExt temp(bitmapa);
// 		unsigned x, y, z;
// 		x = bitmapa.sx();
// 		y = bitmapa.sy();
// 		z = bitmapa.sz();
// 		for(int i=0; i<x; i++) {
// 			for(int j=0; j<y; j++) {
// 				for(int k=0; k<z; k++) {
// 					if(temp(i,j,k)) {
// 						if(i-1 >= 0) {
// 							if(!temp(i-1,j,k)) bitmapa(i,j,k) = 0;
// 						}
// 						if(j-1 >= 0) {
// 							if(!temp(i,j-1,k)) bitmapa(i,j,k) = 0;
// 						}
// 						if(k-1 >= 0) {
// 							if(!temp(i,j,k-1)) bitmapa(i,j,k) = 0;
// 						}
// 						if(i+1 < x) {
// 							if(!temp(i+1,j,k)) bitmapa(i,j,k) = 0;
// 						}
// 						if(j+1 < y) {
// 							if(!temp(i,j+1,k)) bitmapa(i,j,k) = 0;
// 						}
// 						if(k+1 < z) {
// 							if(!temp(i,j,k+1)) bitmapa(i,j,k) = 0;
// 						}
// 					}
// 				}
// 			}
// 		}
// 	}
// };

// class Dylatacja : public Przeksztalcenie {
// public:
// 	void przeksztalc(Bitmapa& bitmapa) {
// 		BitmapaExt temp(bitmapa);
// 		unsigned x, y, z;
// 		x = bitmapa.sx();
// 		y = bitmapa.sy();
// 		z = bitmapa.sz();
// 		for(int i=0; i<x; i++) {
// 			for(int j=0; j<y; j++) {
// 				for(int k=0; k<z; k++) {
// 					if(!temp(i,j,k)) {
// 						if(i-1 >= 0) {
// 							if(temp(i-1,j,k)) bitmapa(i,j,k) = 1;
// 						}
// 						if(j-1 >= 0) {
// 							if(temp(i,j-1,k)) bitmapa(i,j,k) = 1;
// 						}
// 						if(k-1 >= 0) {
// 							if(temp(i,j,k-1)) bitmapa(i,j,k) = 1;
// 						}
// 						if(i+1 < x) {
// 							if(temp(i+1,j,k)) bitmapa(i,j,k) = 1;
// 						}
// 						if(j+1 < y) {
// 							if(temp(i,j+1,k)) bitmapa(i,j,k) = 1;
// 						}
// 						if(k+1 < z) {
// 							if(temp(i,j,k+1)) bitmapa(i,j,k) = 1;
// 						}
// 					}
// 				}
// 			}
// 		}
// 	}
// };

// class Zerowanie : public Przeksztalcenie {
// public:
// 	void przeksztalc(Bitmapa& bitmapa) {
// 		for(int i=0; i<bitmapa.sx(); i++) {
// 			for(int j=0; j<bitmapa.sy(); j++) {
// 				for(int k=0; k<bitmapa.sz(); k++) {
// 					bitmapa(i,j,k) = false;
// 				}
// 			}
// 		}
// 	}
// };

// class Usrednianie : public Przeksztalcenie {
// public:
// 	void przeksztalc(Bitmapa& bitmapa) {
// 		BitmapaExt temp(bitmapa);
// 		unsigned x, y, z;
// 		x = bitmapa.sx();
// 		y = bitmapa.sy();
// 		z = bitmapa.sz();
// 		for(int i=0; i<x; i++) {
// 			for(int j=0; j<y; j++) {
// 				for(int k=0; k<z; k++) {
// 					unsigned biale = 0, czarne = 0;
// 					if(i-1 >= 0) {
// 						if(temp(i-1,j,k)) czarne++;
// 						else biale++;
// 					}
// 					if(j-1 >= 0) {
// 						if(temp(i,j-1,k)) czarne++;
// 						else biale++;
// 					}
// 					if(k-1 >= 0) {
// 						if(temp(i,j,k-1)) czarne++;
// 						else biale++;
// 					}
// 					if(i+1 < x) {
// 						if(temp(i+1,j,k)) czarne++;
// 						else biale++;
// 					}
// 					if(j+1 < y) {
// 						if(temp(i,j+1,k)) czarne++;
// 						else biale++;
// 					}
// 					if(k+1 < z) {
// 						if(temp(i,j,k+1)) czarne++;
// 						else biale++;
// 					}
// 					if(biale > 3) bitmapa(i,j,k) = 0;
// 					if(czarne > 3) bitmapa(i,j,k) = 1;
// 				}
// 			}
// 		}
// 	}
// };

class ZlozeniePrzeksztalcen : public Przeksztalcenie {
public:
	ZlozeniePrzeksztalcen() {}
	void przeksztalc(Bitmapa& bitmapa) {
		for(int i=0; i<przeksztalcenia.size(); i++) { przeksztalcenia[i]->przeksztalc(bitmapa); }
	}
	void dodajPrzeksztalcenie(Przeksztalcenie* p) {
		przeksztalcenia.push_back(p);
	}
	~ZlozeniePrzeksztalcen() {
		przeksztalcenia.clear();
	}

private:
	vector<Przeksztalcenie*> przeksztalcenia;
};