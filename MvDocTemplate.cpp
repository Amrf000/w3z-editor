// Created by: Yog Sothoth
// Company: The Old Ones
// More Info: Azathoth@Cyberdude.com
// Home Page: http://www.geocities.com/SiliconValley/Peaks/2976/

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// @doc
// @module	MvDocTemplate.cpp | 
//			This module allow you to use document with multiple views.  It
//			can be multiple document interface or single document interface.
// @End -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Include file section.
// ------------------------------------------------------------------------
// Precompile header.
#include "stdafx.h"

// Class definition file.
#include "MvDocTemplate.h"

// Afx privae include file.
#include <AfxPriv.h>

#define GET_RTCNAME( Object ) Object->GetRuntimeClass()->m_lpszClassName
// @End -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Constructor.  Those information are directly passed to the
// base class that will manage the document creation.
CSDIMVDocTemplate::CSDIMVDocTemplate(	
		UINT _nIDResource, 
		CRuntimeClass* _pDocClass, 
		BOOL _bAutoDelete /* = TRUE */ )
	  : CMvDocTemplate( _nIDResource, _pDocClass, _bAutoDelete )
{
	// Init value to prevent error.
	m_pDocument = NULL;
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
CSDIMVDocTemplate::~CSDIMVDocTemplate( void )
{
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Document related method.	 Those method are needed by the
// sdi concept.  To support SDI.

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// This method is called by the CDocTemplate class when opening a 
// document.  Because we only have 1 docucument we first close the 
// one we got, the we set the current to the one we receive in 
// parameter.
void CSDIMVDocTemplate::AddDocument( CDocument* _pDoc )
{
	// Validate the new document.
	if ( !_pDoc )
	{
	    ASSERT ( _pDoc );
	    TRACE( "Invalid document parameter in the AddDocument method in %s at %d.\n", THIS_FILE, __LINE__ );
	    return;
	}
	
	// Make sure the document is valid.
	ASSERT_VALID( _pDoc );
	
	// Call the add document method of our parent class. We don't call the 
	// multidoc add document because we don't want a multi doc application.
	CDocTemplate::AddDocument( _pDoc );
	
	// If a document is already open, we must close it.
	if ( m_pDocument )
	{
	    // Already have a document.
	    // Must close it.
	    CloseAllDocuments( FALSE );
	}
	
	// Store the document pointer for later use.
	m_pDocument = _pDoc;
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// This method is called by the CDocTemplate class when closing a 
// document.  Because we only have 1 docucument we only set the 
// current one to NULL.
void CSDIMVDocTemplate::RemoveDocument( CDocument* _pDoc )
{
    // Validate the document to remove.
    if ( !_pDoc )
    {
        ASSERT ( _pDoc );
	    TRACE( "Invalid document parameter in the RemoveDocument method in %s at %d.\n", THIS_FILE, __LINE__ );
        return;
    }

    // Since we only got one document to remove, must be the same we keep.
    else if ( m_pDocument != _pDoc )
    {
        // Not the same.
    	ASSERT( m_pDocument == _pDoc );
        TRACE( "Document to remove is not the same as the one we got in the  \
				RemoveDocument method in %s at %d.\n", THIS_FILE, __LINE__ );
    }
    
    // Make sure the document is valid.
	ASSERT_VALID( _pDoc );

	// Clean the memory.
	CleanDocument( _pDoc );

	// Call the base class method. Call the CDocTemplate since we manage the doc
	// ourselves.
	CDocTemplate::RemoveDocument( _pDoc );

    // Set the current document to NULL to avoid pointer problem.
 	m_pDocument = NULL;
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// This method is called by the CDocTemplate class when managing 
// the document.  Because we only have 1 document,  we can't return
// the position of the first one.  We must return a special ID.
// 
// Return: Special id to identify the first document.
POSITION CSDIMVDocTemplate::GetFirstDocPosition( void ) const
{
 	return ( m_pDocument == NULL ) ? NULL : BEFORE_START_POSITION;
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// This method is called by the CDocTemplate class when managing 
// the document.  Because we only have 1 document,  we only return 
// the currrent document, depending on the position pass in 
// parameter.  When returning, we set the current position to NULL, 
// because there is no more document in the list.
//
// Return: The document pointer.
CDocument* CSDIMVDocTemplate::GetNextDoc( POSITION& _rPos ) const
{
    // Got only 1 document at the same time.  We set the next position to 
    // null. When the framework ask the next document, we will return NULL
    CDocument* pReturnDoc = NULL;
    
    // Validate the position requested.
    if ( _rPos == BEFORE_START_POSITION )
    {
        // The position correspond to the first document, our only one.
        // Return it.
        pReturnDoc = m_pDocument;
    }

    // The position is invalid.
    _rPos = NULL;
    return pReturnDoc;
}
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Constructor.  Those information are directly passed to the
// base class that will manage the document creation.
CMDIMVDocTemplate::CMDIMVDocTemplate(	
		UINT _nIDResource, 
		CRuntimeClass* _pDocClass, 
		BOOL _bAutoDelete /* = TRUE */)
	  : CMvDocTemplate( _nIDResource, _pDocClass, _bAutoDelete )
{
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
CMDIMVDocTemplate::~CMDIMVDocTemplate( void )
{
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// This method is called by the CDocTemplate class when closing a 
// document.  Must overload to clean the allocated memory associated
// with that document.
void CMDIMVDocTemplate::RemoveDocument( CDocument* _pDoc )
{
    // Validate the document to remove.
    if ( !_pDoc )
    {
        ASSERT ( _pDoc );
	    TRACE( "Invalid document parameter in the RemoveDocument method in %s at %d.\n", THIS_FILE, __LINE__ );
        return;
    }

    // Make sure the document is valid.
	ASSERT_VALID( _pDoc );

	// Clean the memory.
	CleanDocument( _pDoc );

	// Call the base class method. Call the CDocTemplate since we manage the doc
	// ourselves.
	CMultiDocTemplate::RemoveDocument( _pDoc );
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Helpfull class to get the document associated document frame manager.
CDocFrameMgr* CMvDocTemplate::GetAssociatedDocFrameMgr( CDocument* _pDoc )
{
	POSITION pos = m_DocumentFrameList.GetHeadPosition();
	while ( pos )
	{
		CDocFrameMgr* pDocFrameMgr = m_DocumentFrameList.GetNext( pos );
		if ( pDocFrameMgr->GetDocument() == _pDoc )
		{
			return pDocFrameMgr;
		}
	}

	// Not found.
	return NULL;
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Constructor.  Those information are directly passed to the
// base class that will manage the document creation.
CMvDocTemplate::CMvDocTemplate(	
		UINT _nIDResource, 
		CRuntimeClass* _pDocClass, 
		BOOL _bAutoDelete /* = TRUE */ )
	 : CMultiDocTemplate( _nIDResource, _pDocClass, NULL, NULL )
{
    m_bAutoDelete = _bAutoDelete;
    m_pFrameToActivate = NULL;
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
CMvDocTemplate::~CMvDocTemplate( void )
{
    // Must clean all memory allocated.
	while ( m_FrameTemplateList.GetCount() )
	{
		delete m_FrameTemplateList.RemoveTail();
	}

    // Must clean all memory allocated.
	while ( m_DocumentFrameList.GetCount() )
	{
		delete m_DocumentFrameList.RemoveTail();
	}
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// We overload the open document method to prepare and open all frame.
CDocument* CMvDocTemplate::OpenDocumentFile( 
		LPCTSTR _lpszPathName, 
	    BOOL bAddToMRU, BOOL _bMakeVisible)
{
    // Create the document instance and validate it.
    CDocument* pDocument = CreateNewDocument();
    if ( !pDocument )
    {
        // The document is invalid.
        // Inform the user of the error, and exit the method.
        AfxMessageBox( AFX_IDP_FAILED_TO_CREATE_DOC );
        TRACE( "Unable of creating the document in OpenDocumentFile \
			    in %s at %d.\n", THIS_FILE, __LINE__ );
        return NULL;
    }

    // Validate the document.
    ASSERT_VALID( pDocument );

    // The document is valid.  Set the autodelete flag.
    pDocument->m_bAutoDelete = m_bAutoDelete;

    // Verify if we are creating a new document, or opening an exesting one.
    if ( _lpszPathName == NULL )
    {
        // Xreate a new document - with default document name
        SetDefaultTitle( pDocument );

        // Avoid creating temporary compound file when starting up invisible
        if ( !_bMakeVisible )
        {
            pDocument->m_bEmbedded = TRUE;
        }

        // Initialize the document.
        if ( !pDocument->OnNewDocument() )
        {
            // Unable to initialize the document.
            TRACE( "Unable to initialize the new document in OpenDocumentFile \
					in %s at %d.\n", THIS_FILE, __LINE__ );
            return NULL;
        }

		// Increment the document counter.
		m_nUntitledCount++;
    }
    else
    {
        // Open an existing document
        CWaitCursor wait;
        if ( !pDocument->OnOpenDocument( _lpszPathName ) )
        {
            // Unable to open the document.
            TRACE( "Unable to open the document in OpenDocumentFile \
					in %s at %d.\n", THIS_FILE, __LINE__ );

            // We must delete the document instance.
            RemoveDocument( pDocument );
            delete pDocument;
            CloseAllDocuments( FALSE );

            return NULL;
        }
        // Set the document file path and name.
        pDocument->SetPathName( _lpszPathName );
    }

    // Now that the document is created, we can create all the associated frame.
	CDocFrameMgr* pDocFrameMgr = new CDocFrameMgr( pDocument, m_FrameTemplateList, m_pFrameToActivate );
	if ( !pDocFrameMgr )
	{
		// Error creating the object.
        TRACE( "Unable to create the associated frame in OpenDocumentFile \
				in %s at %d.\n", THIS_FILE, __LINE__ );

        // We must delete the document instance.
        RemoveDocument( pDocument );
        delete pDocument;
        CloseAllDocuments( FALSE );

        return NULL;
	}

	// Init the frame.
	pDocFrameMgr->OpenStartupFrame();
	m_DocumentFrameList.AddTail( pDocFrameMgr );

    // Return the created document.
    return pDocument;
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// We overload the remove document to remove the associated memory with that
// document.  We must delete the associated docframemanager.
void CMvDocTemplate::CleanDocument( CDocument* _pDoc )
{
    // Validate the document.
    if ( !_pDoc )
    {
        ASSERT ( _pDoc );
        TRACE( "Invalid document parameter in RemoveDocument in \
				in %s at %d.\n", THIS_FILE, __LINE__ );
        return;
    }

    // Make sure the document is valid.
	ASSERT_VALID( _pDoc );

    // Must delete the associated frame manager.
	CDocFrameMgr* pDocFrameMgr = GetAssociatedDocFrameMgr( _pDoc );
	//+Zep...
	POSITION tPos = m_DocumentFrameList.Find( pDocFrameMgr );
	if (NULL != tPos)
	{
		m_DocumentFrameList.RemoveAt( tPos );
	}
	//...+Zep
	//+zep m_DocumentFrameList.RemoveAt( m_DocumentFrameList.Find( pDocFrameMgr ) );
	delete pDocFrameMgr;
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// To add a specific frame template to that kind of document.
void CMvDocTemplate::AddFrameTemplate(	
		CFrameTemplate* _pFrameTemplate, 
		bool _bActivate /* = false */ )
{
    // Validate the needed information.
    if ( !_pFrameTemplate )
    {
        // Must never happen.
        ASSERT ( _pFrameTemplate );
        TRACE( "Invalid frame template specify is the AddFrameTemplate in \
				in %s at %d.\n", THIS_FILE, __LINE__ );
        return;
    }
    
    // Add the frame template in the map index.
    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    m_FrameTemplateList.AddTail( _pFrameTemplate );    

    // Remember the frame template if it's the one to activate.
    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    if ( _bActivate )
    {
        m_pFrameToActivate = _pFrameTemplate;
    }
}
        
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Register the associated icon with the document.  Actually fix a
// problem with the way MS register your document icon in the system.
void CMvDocTemplate::RegisterIconType( void )
{
	// Get the application name.
    char buffer[1024];
    GetModuleFileName( NULL, buffer, 1024 );
    CString sBuffer( buffer );

    // Set the icon key to the resource id value.
    CString Icon = sBuffer + ",-";
    Icon.Format( Icon + "%d", m_nIDResource );

    // Get the doc string.
    CString RegDocName;
    GetDocString( RegDocName, CDocTemplate::regFileTypeId) ;
	RegSetValue( HKEY_CLASSES_ROOT, RegDocName + "\\DefaultIcon", REG_SZ, Icon, lstrlen( Icon ) * sizeof( TCHAR ) );
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Overloaded method to manage the list of frame.
CFrameWnd* CMvDocTemplate::CreateNewFrame(	
		CDocument* _pDoc, 
		CFrameWnd* _pOther )
{
    // Find the correspondant template, and validate it.
	CDocFrameMgr* pDocFrameMgr = GetAssociatedDocFrameMgr( _pDoc );
	CFrame* pChosenFrame = pDocFrameMgr->GetActiveFrame();
    if ( !pChosenFrame )
    {
        // Not found.
        // Not supposed to happen.
        ASSERT( pChosenFrame );
        return NULL;
    }

    // Create the associated window and return it.
	if ( AfxGetMainWnd() && ( ( CMDIFrameWnd* ) AfxGetMainWnd() )->GetActiveFrame() )
	{
	    CDocument* pDoc = ( ( CMDIFrameWnd* ) AfxGetMainWnd() )->GetActiveFrame()->GetActiveDocument();
	    CMDIChildWnd* pNewFrameWnd	= ( CMDIChildWnd* ) ( pChosenFrame->CreateNewFrame( pDoc, NULL ) );
	    return pNewFrameWnd;
	}

	return NULL;
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Microsoft is using the first icon in your exe file as you doc icon.
// If your icon is not the first one, then you are stuck.  Use that method
// instead and it'll fix your problem.
void CMvDocTemplate::InitialUpdateFrame(	
		CFrameWnd* _pFrameWnd, 
		CDocument* _pDoc, 
		BOOL _bMakeVisible /* = TRUE */ )
{
    // Validate the needed information.
    if ( !_pFrameWnd ) 
    {
        // Must never happen.
        ASSERT( _pFrameWnd );
        TRACE( "Invalid frame argument in InitialUpdateFrame.\n", THIS_FILE, __LINE__ );
        return;
    }

    // Make sure that the frame is a mdi child window.
    if ( !_pFrameWnd->IsKindOf( RUNTIME_CLASS( CMDIChildWnd ) ) ) 
    {
        // Must never happen.
        ASSERT( _pFrameWnd->IsKindOf( RUNTIME_CLASS( CMDIChildWnd ) ) );
        TRACE( "Invalid MDI frame in InitialUpdateFrame.\n", THIS_FILE, __LINE__ );
        return;
    }

    // Initialize the newly created frame.
    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	_pFrameWnd->InitialUpdateFrame( _pDoc, _bMakeVisible );
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Overloaded method to open the frame associated with the event id.
BOOL CMvDocTemplate::OnCmdMsg(	
		UINT _nID, int _nCode, void* _pExtra, 
		AFX_CMDHANDLERINFO* _pHandlerInfo )
{
    // If pHandlerInfo is NULL, then handle the message.
    if ( _pHandlerInfo == NULL )
	{
		// We must get the active document.
		CDocument* pDoc = NULL;
		if ( AfxGetMainWnd() && ( ( CMDIFrameWnd* ) AfxGetMainWnd() )->GetActiveFrame() )
		{
			pDoc = ( ( CMDIFrameWnd* ) AfxGetMainWnd() )->GetActiveFrame()->GetActiveDocument();
		}

		// Retrieve the associate doc manager and validate it.
		BOOL bProcessed = FALSE;
		CDocFrameMgr* pDocFrameMgr = GetAssociatedDocFrameMgr( pDoc );
		if ( pDocFrameMgr )
		{
			bProcessed = pDocFrameMgr->OnCmdMsg( _nID, _nCode, _pExtra );
		}

		// Make sure it was processed.
		if ( bProcessed )
		{
			return TRUE;
		}
    }

    // The message was not for us.
    // Call the parent class method.
    return CMultiDocTemplate::OnCmdMsg( _nID, _nCode, _pExtra, _pHandlerInfo );
}
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
      

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// To find the frame template associated to the given class and frame.
CFrame* CDocFrameMgr::FindFrameEx( 
		CString _ViewClassName, 
		CString _FrameClassName )
{
	// Scan the entire list to find the associated frame.
    POSITION Position = m_FrameList.GetHeadPosition();
    while ( Position )
    {
        CFrame* pFrame = m_FrameList.GetNext( Position );
        
        // Validate the frame template.
        if ( !pFrame )
        {
            // Must never happen.
            ASSERT ( pFrame );
            TRACE ( "Invalid frame in FindFrameTemplate \
					 in %s at %d.\n", THIS_FILE, __LINE__ );
        }
        else if ( pFrame->GetViewRTC()->m_lpszClassName == _ViewClassName &&
				  pFrame->GetFrameRTC()->m_lpszClassName == _FrameClassName )
        {
            // Found it.
            return pFrame;
        }
    }

	// Not found.
	return NULL;
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// This constructor build all the required frame based on the frame template
// list.  Also store the frame to activate for later use.
CDocFrameMgr::CDocFrameMgr(	
		CDocument* _pDocument, 
		FrameTemplateList_& _FrameTemplateList, 
		CFrameTemplate* _pFrameToActivate )
{
	// Keep the document pointer for later use.
	m_pDocument = _pDocument;
	m_pFrameToActivate = NULL;

	// Create the frame template for each frame info in the list.
	POSITION pos = _FrameTemplateList.GetHeadPosition( );
	while ( pos )
	{
		CFrameTemplate* pFrameTemplate = _FrameTemplateList.GetNext( pos );

		CFrame* pFrame = new CFrame( pFrameTemplate, m_pDocument );
		m_FrameList.AddTail( pFrame );

		if ( pFrameTemplate == _pFrameToActivate )
		{
			m_pFrameToActivate = pFrame;
		}
	}
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Free the allocated memory in the frame list.
CDocFrameMgr::~CDocFrameMgr( void )
{
	while ( m_FrameList.GetCount() )
	{
		delete m_FrameList.RemoveTail();
	}
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Usefull method to interact with the different frame.
bool CDocFrameMgr::OpenStartupFrame( void )
{
    // Local definition.
    CFrameWnd* WndToActivate = NULL;

    // Scan the map and open the frame that must be open
    // at startup.
    POSITION Position = m_FrameList.GetHeadPosition();
    while ( Position )
    {
        CFrame* pFrame = m_FrameList.GetNext( Position );
        
        if ( pFrame->GetLoadAtStartup() )
        {
            // The frame must be loaded when the document is being open.
            // Create the frame instance and validate it.
            CFrameWnd* pFrameWnd = pFrame->CreateFrame();
            if ( pFrameWnd )
            {
                // The frame is now created, we must activate it.
                pFrameWnd->ShowWindow( pFrame->GetDefaultWndStatus() );

				if ( ( m_pFrameToActivate == pFrame ) ||
					   !m_pFrameToActivate )
				{
                    WndToActivate = pFrameWnd;
				}
            }
        }
    }

    // Activate the window.
    if ( WndToActivate )
    {
        WndToActivate->ActivateFrame();
        WndToActivate->SetFocus();
    }

	return true;
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Will return you the frame that is currently activated.
CFrame* CDocFrameMgr::GetActiveFrame( void )
{
    // Validate the needed information.
    if( !AfxGetMainWnd() )
    {
        // The main window is not valid, or the current active frame is the main window.
        // This mean that no child frame are currently open.  Must never happen.
        ASSERT( AfxGetMainWnd() );
        TRACE( "Invalid main window in CreateNewFrame.\n", THIS_FILE, __LINE__ );
        return NULL;
    }
    else if ( !( ( CFrameWnd* ) AfxGetMainWnd() )->GetActiveFrame() )
    {
        // Must never happen.
        ASSERT( ( ( CFrameWnd* ) AfxGetMainWnd() )->GetActiveFrame() );
        TRACE( "Invalid active frame in CreateNewFrame.\n", THIS_FILE, __LINE__ );
        return NULL;
    }
    else if ( ( ( ( CFrameWnd* ) AfxGetMainWnd() )->GetActiveFrame() == AfxGetMainWnd() ) )
    {
        ASSERT( ( ( CFrameWnd* ) AfxGetMainWnd() )->GetActiveFrame() == AfxGetMainWnd() );
        TRACE( "Invalid main window in CreateNewFrame.\n", THIS_FILE, __LINE__ );
        return NULL;
    }
    else if ( !( ( CFrameWnd* ) AfxGetMainWnd() )->GetActiveFrame()->GetActiveView() )
    {
        // Must never happen.
        ASSERT( ( ( CFrameWnd* ) AfxGetMainWnd() )->GetActiveFrame()->GetActiveView() );
        TRACE( "Invalid active view in CreateNewFrame.\n", THIS_FILE, __LINE__ );
        return NULL;
    }

    // Find the correspondant template, and validate it.
    CFrameWnd* pChosenFrame = ( ( CFrameWnd* ) AfxGetMainWnd() )->GetActiveFrame();
    CView* pChosenView = pChosenFrame->GetActiveView();
    return FindFrame( pChosenView, pChosenFrame );
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Will display the requested frame.
void CDocFrameMgr::ShowOrCreateFrame( UINT _nEventID )
{
    // Verify if the window is already created.  If not, create it.
    CFrame* pFrame = FindFrame( _nEventID );
	if ( pFrame )
	{
		pFrame->ShowOrCreateFrame();
	}
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Search method based on different criteria.
CFrame* CDocFrameMgr::FindFrame( CString _ViewClassName, CString _FrameClassName )
{
    // Try to find it directly.
    CFrame* pFrame = FindFrameEx( _ViewClassName, _FrameClassName );
    if ( pFrame )
    {
        // Found it.
        return pFrame;
    }

    // Didn't find the frame.
    // Probably a frame with more than one view.  
    // The focused view may not be the one registered.
    // Search all view, and try to find the one that match the frame.
    POSITION Position = m_pDocument->GetFirstViewPosition();
    while ( Position != NULL )
    {
        // Retrieve the view at the given position.
        CView* pView = m_pDocument->GetNextView( Position );

        // Verify that the view is associated with our frames and 
		// make sure we do not check the given class name, since it
		// was check before.
        if ( GET_RTCNAME ( pView->GetParentFrame() ) == _FrameClassName && 
             GET_RTCNAME ( pView ) != _ViewClassName )
        {
            // Try to find it.
            pFrame = FindFrameEx( GET_RTCNAME ( pView ), _FrameClassName );
            if ( pFrame )
            {
                // Got it.
                return pFrame;
            }
        }
    }
    
    // Not found.
    return NULL;
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// That method only call the other find frame method with the required param.
CFrame* CDocFrameMgr::FindFrame( CWnd* _pView, CWnd* _pFrame )
{
	// Make sure the parameter are valid.
	if ( !_pView || !_pFrame )
	{
		ASSERT( _pView );
		ASSERT( _pFrame );
        TRACE( "Invalid pointer pass if FindFrameTemplate \
				in %s at %d.\n", THIS_FILE, __LINE__ );
	}

	// Call the other method using the appropriate parameter.
    return FindFrame( GET_RTCNAME( _pView ), GET_RTCNAME( _pFrame ) );
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Scan the list of frame to find the one associate with that id.
CFrame* CDocFrameMgr::FindFrame( UINT _nEventId )
{
	// Scan the entire list to find the associated frame.
    POSITION Position = m_FrameList.GetHeadPosition();
    while ( Position )
    {
        CFrame* pFrame = m_FrameList.GetNext( Position );
        
        // Validate the frame template.
        if ( !pFrame )
        {
            // Must never happen.
            ASSERT ( pFrame );
            TRACE ( "Invalid frame in FindFrameTemplate \
					 in %s at %d.\n", THIS_FILE, __LINE__ );
        }
        else if ( pFrame->GetEventID() == _nEventId )
        {
            // Found it.
            return pFrame;
        }
    }

	// Not found.
	return NULL;
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Method to open the frame associated with the event id.
BOOL CDocFrameMgr::OnCmdMsg( UINT _nID, int _nCode, void* _pExtra )
{
	CFrame* pChosenFrame = FindFrame( _nID );
    if ( _nCode == CN_COMMAND && pChosenFrame )
    {
        // Try to create the frame.
        return pChosenFrame->ShowOrCreateFrame(  );
    }
    else if ( _nCode == CN_UPDATE_COMMAND_UI && pChosenFrame )
    {
        // This is an Update UI event and the id is a frame open id.
        // Enable the menu item.
        ( ( CCmdUI* ) _pExtra )->Enable( TRUE );

        // Find the currently selected window.
        if ( ( ( CFrameWnd* ) AfxGetMainWnd() )->GetActiveFrame() != AfxGetMainWnd() )
        {
            CFrameWnd* pChosenFrameWnd = ( ( CFrameWnd* ) AfxGetMainWnd() )->GetActiveFrame();
            CView* pChosenView = pChosenFrameWnd->GetActiveView();

            CFrame* pChosenFrame = FindFrame( pChosenView, pChosenFrameWnd );
            ( ( CCmdUI* ) _pExtra )->SetCheck( pChosenFrame->GetEventID() == _nID );
        }
        else
        {
            ( ( CCmdUI* ) _pExtra )->SetCheck( FALSE );
        }

        // Tell the framework that the message has been handle.
        return TRUE;
    }
	return FALSE;
}
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

IMPLEMENT_DYNAMIC( CFrame, CMultiDocTemplate )
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Store the needed value.
CFrame::CFrame( CFrameTemplate* _pFrameTemplate, CDocument* _pDocument )
     : CMultiDocTemplate( _pFrameTemplate->GetResourceID(), NULL, 
						  _pFrameTemplate->GetFrameRTC(), 
						  _pFrameTemplate->GetViewRTC() )
{
	m_pFrameTemplate = _pFrameTemplate;
	m_pDocument = _pDocument;
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
CFrame::~CFrame( void )
{
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Functions used by the framework to manage the window once created.
// Will create the frame window.
CFrameWnd* CFrame::CreateFrame( void )
{
    // Create the window and validate it.
    CMDIChildWnd* pNewFrameWnd	= ( CMDIChildWnd* )( CreateNewFrame( m_pDocument, NULL ) );
    if ( !pNewFrameWnd ) 
    {
        // Must never happen.
        ASSERT( pNewFrameWnd );
        TRACE( "Unable to create frame wnd in CFrameTemplate::CreateFrame.\n", THIS_FILE, __LINE__ ); 
        return NULL;
    }
 
    // Make sure that the frame is a mdi child window.
    ASSERT( pNewFrameWnd->IsKindOf( RUNTIME_CLASS( CMDIChildWnd ) ) );

    // Initialize the newly created frame.
    InitialUpdateFrame( pNewFrameWnd, m_pDocument, FALSE );
	pNewFrameWnd->SendMessageToDescendants( WM_INITIALUPDATE, 0, 0, TRUE, TRUE );

    // Return the new frame.
    return pNewFrameWnd;
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Will destroy the associated window.
bool CFrame::DestroyFrame( void )
{
	// Find the window and close it.
    CFrameWnd* pWnd = FindFrameWnd( );
	if ( pWnd )
	{
	    pWnd->DestroyWindow();
	}

    return true;
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Will display the window.
bool CFrame::ShowOrCreateFrame( void )
{
	CFrameWnd* pFrameWnd = FindFrameWnd();
    if ( !pFrameWnd ) 
    {
        // Frame not open.  Create it and validate it.
        pFrameWnd = CreateFrame();
        if ( !pFrameWnd ) 
        {
            // Must never happen.
            ASSERT( pFrameWnd );
            TRACE( "Unable to create frame in ShowOrCreateFrame in %s at %d.\n", THIS_FILE, __LINE__ );
            return false;
        }
    }
    
    // Retrieve the current window states.
    int nStates = SW_RESTORE;
    CWnd* pActWnd = ( ( CFrameWnd* ) AfxGetMainWnd() )->GetActiveFrame();
    if ( pActWnd ) 
    {
        WINDOWPLACEMENT wp;
        pActWnd->GetWindowPlacement( &wp );
        if ( wp.showCmd != SW_SHOWMINIMIZED )
        {
            nStates = wp.showCmd;
        }
    }

    // Activate the frame in the given states.
    pFrameWnd->ActivateFrame( nStates );
    return true;
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Will return you it's associated window.
CFrameWnd* CFrame::FindFrameWnd( void )
{
    // Validate the needed information.
    if ( !m_pDocument ) 
    {
        ASSERT ( m_pDocument );
        TRACE( "Invalid document while searching for a window in FindFrame in %s at %d.\n", THIS_FILE, __LINE__ );
    }
    
    // Search the document to find the associted frame window.
    POSITION Position = m_pDocument->GetFirstViewPosition();
    while ( Position != NULL )
    {
        // Retrieve the view at the given position.
        CView* pView = m_pDocument->GetNextView( Position );

        // Verify to see if the view and the frame runtime class
        // correspond to the frame template we are searching.
        if ( pView->IsKindOf( m_pFrameTemplate->GetViewRTC() ) &&
             pView->GetParentFrame()->IsKindOf( m_pFrameTemplate->GetFrameRTC() ) )
        {
            // Got it.
            return pView->GetParentFrame();
        }
    }

    // Not found.
    return NULL;
}
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-



