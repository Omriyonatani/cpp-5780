/**
 * Specialization of a vector class to handle vector<bool> more efficiently.
 * 
 * @author  Peter Gottschling
 * @url     https://github.com/petergottschling/discovering_modern_cpp/blob/master/c%2B%2B11/derivative.cpp
 */

#include <cassert>
#include <iostream>
#include <memory>
#include <algorithm>


/*
 * PART A: Generic vector class
 */

template <typename T>
class vector {
    int my_size;
    T*  data;
    void check_index(int i) const { assert(i >= 0 && i < my_size); }

  public:
    explicit vector(int size): 
      my_size(size), data( new T[my_size] )  {}

    vector(): my_size(0), data(nullptr) {}
    ~vector() {delete[] data;}

    vector(const vector& that): 
      my_size(that.my_size), data(new T[my_size])  {
    	  std::copy(&that.data[0], &that.data[that.my_size], &data[0]);
    }

    vector& operator=(const vector& that) {
      assert(my_size == that.my_size);
      std::copy(&that.data[0], &that.data[that.my_size], &data[0]);
      return *this;
    }

    int size() const { return my_size; }

    const T& operator[](int i) const {
        check_index(i);
        return data[i];
    }

    T& operator[](int i) {
        check_index(i);
        return data[i];
    }

    vector operator+(const vector& that) const {
        assert(my_size == that.my_size);
        vector sum(my_size);
        for (int i= 0; i < my_size; ++i) 
            sum[i] = data[i] + that[i];
        return sum;
    }
};


template <typename T>
std::ostream& operator<<(std::ostream& os, const vector<T>& v) {
  os << '[';
  for (int i= 0; i < v.size(); ++i) 
    os << v[i] << ',';
  os << ']';
  return os;
}




/*
 * PART B: Specialization of vector to bool
 */


/**
 * A proxy to a bit - represents a reference to a single bit inside a char.
 */
class bool_reference {
  private:
    unsigned char& byte;
    unsigned char  mask;
    
  public:
    bool_reference(unsigned char& containing_byte, 
      int bit_index): 
      byte(containing_byte), 
      mask(1 << bit_index) {}

    operator bool() const { 
      return byte & mask; 
    }

    bool_reference& operator=(bool b) { 
        if (b==true)
            byte |= mask;
        else // b==false
            byte &= ~mask;
        return *this; 
    }

};

template <>
class vector<bool> {
    int      my_size;
    uint8_t* data;
    void check_index(int i) const { assert(i >= 0 && i < my_size); }
  public:
    explicit vector(int size):
      my_size(size),
      data(new uint8_t[(my_size+7) / 8]) {}

    vector(): my_size(0), data(nullptr) {}
  
    bool operator[](int i) const { 
      check_index(i);
      return (data[i/8] >> i%8) & 1; 
    }

    bool_reference operator[](int i) { 
      check_index(i);
      return bool_reference{data[i/8], i%8};   
    }

    int size() const { return my_size; }
    // NOTE: size() is not "inherited" from the generic vector!
    // You must define it, with all other methods,
    // in each specialization!
};





int main() {
    vector<float> v( 4 );
    v[0]= v[1]= 1.0; v[2]= 2.0; v[3] = -3.0;

    std::cout << "v = " << v << std::endl;

    vector<float> w(v.size());
    w = v;

    vector<float> sum( w + v );
    std::cout << "w + v = " << sum << std::endl;

    vector<float> z( v );


    vector<bool>  b(13);
    for (int i= 0; i < 13; i++)
		  b[i] = i % 3;
    std::cout << "b = " << b << std::endl;
    for (int i= 0; i < 13; i++)
		  std::cout << b[i];
    std::cout << '\n';

    return 0;
}
