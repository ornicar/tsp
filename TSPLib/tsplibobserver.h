
#ifndef _TSP_LIB_OBSERVER_H_
#define _TSP_LIB_OBSERVER_H_

#include <list>			// for list<>
#include <algorithm>	// for find()
using namespace std;

struct TSPLibObserved ;

struct TSPLibObserver
{
	virtual ~TSPLibObserver(){}
	virtual void Update(TSPLibObserved * pSujet)=0;
};

typedef list<TSPLibObserver*> TListObs;
struct TSPLibObserved
{
	virtual ~TSPLibObserved(){}
	virtual void Attach( TSPLibObserver * );
	virtual void Detach( TSPLibObserver * );
	virtual void Notify();
protected:
	TSPLibObserved(){}
private:
	TListObs m_ListObs;
};

#endif // _TSP_LIB_OBSERVER_H_
