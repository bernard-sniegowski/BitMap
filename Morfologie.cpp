#include <iostream>
#include <vector>

using namespace std;

class Bitmapa {
public:
	virtual unsigned sx() const = 0;
	virtual unsigned sy() const = 0;
	virtual unsigned sz() const = 0;

	virtual bool& operator()(unsigned x, unsigned y, unsigned z) = 0;
	virtual bool operator()(unsigned x, unsigned y, unsigned z) const = 0;

	virtual ~Bitmapa(){}
};

class BitmapaExt : public Bitmapa {
public:
	BitmapaExt(unsigned zakresX, unsigned zakresY, unsigned zakresZ) : x(zakresX), y(zakresY), z(zakresZ), size(x*y*z) {
		bitmapa = new bool[size];
		// for(int i = 0; i<size; i++) bitmapa[i] = false;
		// std::fill_n(bitmapa, size, false);
	}
	BitmapaExt(const Bitmapa& bitmapa) : Bitmapa() {
		this->x = bitmapa.sx();
		this->y = bitmapa.sy();
		this->z = bitmapa.sz();
		this->size = x*y*z;
		this->bitmapa = new bool[this->size];
		// memcpy(((bool *) this->bitmapa), ((bool *) bitmapa.bitmapa), size);
		for(int i=0; i<x; i++) {
			for(int j=0; j<y; j++) {
				for(int k=0; k<z; k++) {
					this->operator()(i,j,k) = bitmapa(i,j,k);
				}
			}
		}
	}
	BitmapaExt(const BitmapaExt& bitmapa) : Bitmapa() {
		this->x = bitmapa.sx();
		this->y = bitmapa.sy();
		this->z = bitmapa.sz();
		this->size = x*y*z;
		this->bitmapa = new bool[this->size];
		memcpy(((bool *) this->bitmapa), ((bool *) bitmapa.bitmapa), size);
		// for(int i=0; i<x; i++) {
		// 	for(int j=0; j<y; j++) {
		// 		for(int k=0; k<z; k++) {
		// 			this->operator()(i,j,k) = bitmapa(i,j,k);
		// 		}
		// 	}
		// }
	}

	BitmapaExt& operator=(const BitmapaExt& bitmapa) {
		delete[] this->bitmapa;
		x = bitmapa.sx();
		y = bitmapa.sy();
		z = bitmapa.sz();
		size = bitmapa.size;
		this->bitmapa = new bool[x*y*z];
		memcpy(((bool *) this->bitmapa), ((bool *) bitmapa.bitmapa), size);
		// for(int i=0; i<x; i++) {
		// 	for(int j=0; j<y; j++) {
		// 		for(int k=0; k<z; k++) {
		// 			this->operator()(i,j,k) = bitmapa(i,j,k);
		// 		}
		// 	}
		// }
		return *this;
	}

	unsigned sx() const { return x; }
	unsigned sy() const { return y; }
	unsigned sz() const { return z; }

	bool& operator()(unsigned i, unsigned j, unsigned k) { return bitmapa[x*y*k + x*j + i]; }
	bool operator()(unsigned i, unsigned j, unsigned k) const { return bitmapa[x*y*k + x*j + i]; }

	~BitmapaExt(){
		delete[] bitmapa;
	}


private:
	unsigned x,y,z;
	bool* bitmapa;
	unsigned size;
};

inline ostream& operator<<(ostream& os, const Bitmapa& b){
    unsigned int x = b.sx();
    unsigned int y = b.sy();
    unsigned int z = b.sz();
    os << "{" << "\n";
    for (unsigned int i = 0; i < x; i++) {
        os << " {" << "\n";
        for (unsigned int j = 0; j < y; j++) {
            os << "  {";
            for (unsigned int k = 0; k < z; k++)
                if (k != z - 1)
                    os << b(i, j, k) << ",";
                else
                    os << b(i, j, k);
            if (j != y - 1)
                os << "}," << "\n";
            else
                os << "}" << "\n";
        }
        if (i != x - 1)
            os << " }," << "\n";
        else
            os << " }" << "\n";
    }
    os<< "}";
    return os;
}

class Przeksztalcenie {
public:
	virtual void przeksztalc( Bitmapa& ) = 0;
	virtual ~Przeksztalcenie() {}
};

class Inwersja : public Przeksztalcenie {
public:
	void przeksztalc(Bitmapa& bitmapa) {
		for(int i=0; i<bitmapa.sx(); i++) {
			for(int j=0; j<bitmapa.sy(); j++) {
				for(int k=0; k<bitmapa.sz(); k++) {
					bitmapa(i,j,k) = !bitmapa(i,j,k);
				}
			}
		}
	}
};

class Erozja : public Przeksztalcenie {
public:
	void przeksztalc(Bitmapa& bitmapa) {
		BitmapaExt temp(bitmapa);
		unsigned x, y, z;
		x = bitmapa.sx();
		y = bitmapa.sy();
		z = bitmapa.sz();
		for(int i=0; i<x; i++) {
			for(int j=0; j<y; j++) {
				for(int k=0; k<z; k++) {
					if(temp(i,j,k)) {
						if(i-1 >= 0) {
							if(!temp(i-1,j,k)) bitmapa(i,j,k) = 0;
						}
						if(j-1 >= 0) {
							if(!temp(i,j-1,k)) bitmapa(i,j,k) = 0;
						}
						if(k-1 >= 0) {
							if(!temp(i,j,k-1)) bitmapa(i,j,k) = 0;
						}
						if(i+1 < x) {
							if(!temp(i+1,j,k)) bitmapa(i,j,k) = 0;
						}
						if(j+1 < y) {
							if(!temp(i,j+1,k)) bitmapa(i,j,k) = 0;
						}
						if(k+1 < z) {
							if(!temp(i,j,k+1)) bitmapa(i,j,k) = 0;
						}
					}
				}
			}
		}
	}
};

class Dylatacja : public Przeksztalcenie {
public:
	void przeksztalc(Bitmapa& bitmapa) {
		BitmapaExt temp(bitmapa);
		unsigned x, y, z;
		x = bitmapa.sx();
		y = bitmapa.sy();
		z = bitmapa.sz();
		for(int i=0; i<x; i++) {
			for(int j=0; j<y; j++) {
				for(int k=0; k<z; k++) {
					if(!temp(i,j,k)) {
						if(i-1 >= 0) {
							if(temp(i-1,j,k)) bitmapa(i,j,k) = 1;
						}
						if(j-1 >= 0) {
							if(temp(i,j-1,k)) bitmapa(i,j,k) = 1;
						}
						if(k-1 >= 0) {
							if(temp(i,j,k-1)) bitmapa(i,j,k) = 1;
						}
						if(i+1 < x) {
							if(temp(i+1,j,k)) bitmapa(i,j,k) = 1;
						}
						if(j+1 < y) {
							if(temp(i,j+1,k)) bitmapa(i,j,k) = 1;
						}
						if(k+1 < z) {
							if(temp(i,j,k+1)) bitmapa(i,j,k) = 1;
						}
					}
				}
			}
		}
	}
};

class Zerowanie : public Przeksztalcenie {
public:
	void przeksztalc(Bitmapa& bitmapa) {
		for(int i=0; i<bitmapa.sx(); i++) {
			for(int j=0; j<bitmapa.sy(); j++) {
				for(int k=0; k<bitmapa.sz(); k++) {
					bitmapa(i,j,k) = false;
				}
			}
		}
	}
};

class Usrednianie : public Przeksztalcenie {
public:
	void przeksztalc(Bitmapa& bitmapa) {
		BitmapaExt temp(bitmapa);
		unsigned x, y, z;
		x = bitmapa.sx();
		y = bitmapa.sy();
		z = bitmapa.sz();
		for(int i=0; i<x; i++) {
			for(int j=0; j<y; j++) {
				for(int k=0; k<z; k++) {
					unsigned biale = 0, czarne = 0;
					if(i-1 >= 0) {
						if(temp(i-1,j,k)) czarne++;
						else biale++;
					}
					if(j-1 >= 0) {
						if(temp(i,j-1,k)) czarne++;
						else biale++;
					}
					if(k-1 >= 0) {
						if(temp(i,j,k-1)) czarne++;
						else biale++;
					}
					if(i+1 < x) {
						if(temp(i+1,j,k)) czarne++;
						else biale++;
					}
					if(j+1 < y) {
						if(temp(i,j+1,k)) czarne++;
						else biale++;
					}
					if(k+1 < z) {
						if(temp(i,j,k+1)) czarne++;
						else biale++;
					}
					if(biale > 3) bitmapa(i,j,k) = 0;
					if(czarne > 3) bitmapa(i,j,k) = 1;
				}
			}
		}
	}
};

class ZlozeniePrzeksztalcen : public Przeksztalcenie {
public:
	void przeksztalc(Bitmapa& bitmapa) {
		for(int i=0; i<przeksztalcenia.size(); i++) { przeksztalcenia[i]->przeksztalc(bitmapa); }
	}
	void dodajPrzeksztalcenie(Przeksztalcenie* p) {
		przeksztalcenia.push_back(p);
	}

private:
	vector<Przeksztalcenie*> przeksztalcenia;
};


int main() {
	BitmapaExt b(3,4,5);
	b(0,1,0)= b(0,2,2) = b(0,2,3) = b(0,1,2)= true;
	b(1,2,1) = b(1,2,2) = b(1,2,3) = b(1,1,1) = b(2,1,2)= true;
	b(2,2,1) = b(2,2,2) = b(2,2,3) = b(2,1,1) = true;

	std::cout << "Oryginalna bitmapa:\n" <<  b << "\n\n";

	Usrednianie u;
	Dylatacja d;
	Inwersja i;
	Zerowanie zerowanie;
	Erozja e;

	ZlozeniePrzeksztalcen z;
	z.dodajPrzeksztalcenie(&u);
	z.dodajPrzeksztalcenie(&d);
	// z.dodajPrzeksztalcenie(&zerowanie);
	// z.dodajPrzeksztalcenie(&i);
	// z.dodajPrzeksztalcenie(&e);

	// przeksztalcenie 'z' najpierw wykona usrednianie, a potem dylatacje
	z.przeksztalc(b);

	std::cout << "Przeksztalcona bitmapa:\n" << b << "\n";

	return 0;
}
