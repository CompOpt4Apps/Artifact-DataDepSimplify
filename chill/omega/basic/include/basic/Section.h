#if ! defined _Section_h
#define _Section_h 1
/*
  Section of an existing collection viewed as a collection
  */

#include <basic/Collection.h>

namespace omega {

template<class T> class Section_Iterator;

template <class T> class Section : public Sequence<T> {
public:
    Section(Sequence<T> *, int start, int length);

    Iterator<T> *new_iterator();

    const T &operator[](int) const;
          T &operator[](int);      

    int index(const T &) const;
    int size() const;

    friend class Section_Iterator<T>;

private:
    Sequence<T> *it;
    int _start, _length;
};

template <class T> class Section_Iterator : public Iterator<T> {
public:
               Section_Iterator(Section<T> &sec);
       virtual ~Section_Iterator() { delete it; }

    const T &  operator*() const	{ return *(*it); }
          T &  operator*()		{ return *(*it); }

       void    operator++(int);
       void    operator++();
    
       bool    live() const;
  Iterator<T> *new_copy() const;

private:
    Section_Iterator(const Section_Iterator<T> &si);
    Iterator<T> *it;
    int remaining;
};

} // namespace

#if ! defined DONT_INCLUDE_TEMPLATE_CODE
#include <basic/Section.c>
#endif

#define instantiate_Section(T)	template class Section<T>; \
				template class Section_Iterator<T>; \
				instantiate_Sequence(T)
#define instantiate_Section_Iterator(T)	  instantiate_Section(T)

#endif
