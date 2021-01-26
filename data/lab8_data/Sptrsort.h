#include <vector>
#include <algorithm>

template <typename T>
class sptr {
	public:
		sptr(T *_ptr=NULL) { ptr = _ptr; }

		bool operator< (const sptr &rhs) const {
			return *ptr < *rhs.ptr;
		}

		operator T * () const { return ptr; }

	private:
		T * ptr;
};

template <typename T>
void sptrsort(std::vector<T *> &A) {
	//USE std::sort on SPTR VERSION OF A
	//NOTE: A is vector of pointers (T *)
	std::vector< sptr<T> > sort_tmp;
	sort_tmp.resize(A.size());

	for (int i = 0; i < (int)A.size(); i++)
		sort_tmp[i] = sptr<T>(A[i]);

	std::sort(sort_tmp.begin(), sort_tmp.end());

	for (int i = 0; i < (int)sort_tmp.size(); i++)
		A[i] = &(*sort_tmp[i]);
}
