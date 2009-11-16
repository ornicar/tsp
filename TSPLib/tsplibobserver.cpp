
#include "tsplibobserver.h"

//----------------------------------------------------------------------------------
void
TSPLibObserved::Attach( TSPLibObserver * pObserver)
{
	m_ListObs.push_front( pObserver );
}

//----------------------------------------------------------------------------------
void
TSPLibObserved::Detach( TSPLibObserver * pObserver)
{
	TListObs::iterator it = find( m_ListObs.begin() , m_ListObs.end(), pObserver );
	if( it != m_ListObs.end() )
		m_ListObs.erase(it);
}

//----------------------------------------------------------------------------------
void
TSPLibObserved::Notify( void )
{
	TListObs::iterator it;
	for( it = m_ListObs.begin() ; it != m_ListObs.end() ; it++ )
		(*it)->Update(this);
}
