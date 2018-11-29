// Created by: Yog Sothoth
// Company: The Old Ones
// More Info: Azathoth@Cyberdude.com
// Home Page: http://www.geocities.com/SiliconValley/Peaks/2976/

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// @doc
// @module	MvDocTemplate.hpp | 
//			This module allow you to use document with multiple views.  It
//			can be multiple document interface or single document interface.
// @End -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Definition to avoid multiple include.
// ------------------------------------------------------------------------
#if !defined( _MvDocTemplate_H_ )
#define _MvDocTemplate_H_ 
// @End -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Include file section.
// ------------------------------------------------------------------------
// Template include file.
#include <afxtempl.h>
// @End -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Definition.
// ------------------------------------------------------------------------
class CSDIMVDocTemplate;	// The single document interface class.
class CMDIMVDocTemplate;	// The multi document interface class.
class CMvDocTemplate;		// The document interface base class.

class CDocFrameMgr;			// Manage the list of open frame for a specific doc.
class CFrameTemplate;		// Specific frame information.
class CFrame;				// The frame itself.

// Doing some definition to simplify the code.  The reason why I didn't use
// stl list is to keep that extension MFC specific only and to avoid adding
// an overload to it.
typedef CList< CFrame*, CFrame*& > FrameList_;
typedef CList< CDocFrameMgr*, CDocFrameMgr*& > DocumentFrameList_;
typedef CList< CFrameTemplate*, CFrameTemplate*& > FrameTemplateList_;
// @End -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Class declaration.
// ------------------------------------------------------------------------
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// @class:  (FUNCTIONAL_CLASS)
//          This is used to define each specific frame that are associated
//			with your document template.
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
class CFrameTemplate
{
    // --------------------------------------------------------------------
    // @group Data definition.
    // --------------------------------------------------------------------
    private:
		CRuntimeClass* m_pFrameClass;	// To be able to create the frame instance.
		CRuntimeClass* m_pViewClass;	// To be able to create the view instance.
		UINT m_nResourceID;				// The associated resource id for menu, icon, ...
		UINT m_nEventID;				// The associated event id used to open or create the window.
		UINT m_nDefaultWndStatus;		// The status of the window at load time.
		BOOL m_bLoadAtStartup;			// To allow loading the window later.

    public:
    protected:

    // --------------------------------------------------------------------
    // @group Implementation Method definition.
    // --------------------------------------------------------------------
	private:
	public:
		// Constructor.  The only required information is the resource id, the
		// frame RTC, the view RTC and the event ID so that you can display your
		// window.
		CFrameTemplate(
            UINT _nIDResource, CRuntimeClass* _pFrameClass, CRuntimeClass* _pViewClass,
            UINT _nEventID, UINT _nDefaultWndStatus = WS_VISIBLE, 
			BOOL _bLoadAtStartup = FALSE )
		{
			m_nResourceID		= _nIDResource;
			m_pFrameClass		= _pFrameClass;
			m_pViewClass		= _pViewClass;
			m_nEventID			= _nEventID;
			m_nDefaultWndStatus	= _nDefaultWndStatus;
			m_bLoadAtStartup	= _bLoadAtStartup;
		}	

		// Wrapper functions to get object attributes.
		// No need for set functions since everything is provide by the constructor
		// and that the concept of document template is static for an application.
		CRuntimeClass* GetFrameRTC( void )	 	{ return m_pFrameClass; };
		CRuntimeClass* GetViewRTC( void )		{ return m_pViewClass; };
		UINT GetResourceID( void )		 		{ return m_nResourceID; };
		UINT GetEventID( void )		 			{ return m_nEventID; };
		UINT GetDefaultWndStatus( void )		{ return m_nDefaultWndStatus; };
		BOOL GetLoadAtStartup( void )			{ return m_bLoadAtStartup; };

	protected:
};

// Starting from here you will find class that are private to the different
// class declore before.  You don't need to use those class.  In fact they
// should have been put in the private declaration of the class that are
// using them.

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// @class:  (IMPLEMENTATION_CLASS)
//			This is the base class for the doc template define before. Will
//			manage a document manager instance for each document open.
// @base    public | CMultiDocTemplate
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
class CMvDocTemplate : public CMultiDocTemplate
{
    // --------------------------------------------------------------------
    // @group Macro definition.
    // --------------------------------------------------------------------

    // --------------------------------------------------------------------
    // @group Data definition.
    // --------------------------------------------------------------------
    private:
    public:
    protected:
        CFrameTemplate* m_pFrameToActivate;		// The frame to show on top when opening doc.
        BOOL m_bAutoDelete;						// Flag to know if we must delete the doc when 
												// all window are closed.
        
        FrameTemplateList_ m_FrameTemplateList;	// The list of frame to open with the document.
		DocumentFrameList_ m_DocumentFrameList;	// The list of document currently open for
												// that type of document.

    // --------------------------------------------------------------------
    // @group Implementation Method definition.
    // --------------------------------------------------------------------
    private:
		// Helpfull class to get the document associated document frame manager.
		CDocFrameMgr* GetAssociatedDocFrameMgr( CDocument* _pDoc );

    public:
		// Constructor.  Those information are directly passed to the
		// base class that will manage the document creation.
        CMvDocTemplate(	UINT _nIDResource, CRuntimeClass* _pDocClass, 
						BOOL _bAutoDelete = TRUE );
        virtual ~CMvDocTemplate( void );

        // We overload the open document method to prepare and open all frame.
        virtual CDocument* OpenDocumentFile( LPCTSTR _lpszPathName, BOOL bAddToMRU, BOOL bMakeVisible);

		//virtual CDocument* OpenDocumentFile(LPCTSTR lpszPathName, BOOL bMakeVisible = TRUE);
		//virtual CDocument* OpenDocumentFile(LPCTSTR lpszPathName, BOOL bAddToMRU, BOOL bMakeVisible);

    protected:
		// When removing a document, we must clean the associated allocated
		// memory.
        virtual void CleanDocument( CDocument* _pDoc );

    // --------------------------------------------------------------------
    // @group Funcional Method definition.
    // --------------------------------------------------------------------
    private:
    public:
        // To add a specific frame template to that kind of document.
        virtual void AddFrameTemplate(	CFrameTemplate* _pFrameTemplate, 
										bool _bActivate = false );
        
        // Register the associated icon with the document.  Actually fix a
		// problem with the way MS register your document icon in the system.
        void RegisterIconType( void );

    protected:

    // --------------------------------------------------------------------
    // @group MFC Definition.
    // --------------------------------------------------------------------
    private:
    public:
        // Overloaded method to manage the list of frame.
        CFrameWnd* CreateNewFrame(	CDocument* _pDoc, CFrameWnd* _pOther );
        void InitialUpdateFrame(	CFrameWnd* _pFrame, CDocument* _pDoc, 
									BOOL _bMakeVisible = TRUE );

		// Overloaded method to open the frame associated with the event id.
        BOOL OnCmdMsg(	UINT _nID, int _nCode, void* _pExtra, 
						AFX_CMDHANDLERINFO* _pHandlerInfo );
        
    protected:
};

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// @class:  (IMPLEMENTATION_CLASS)
//          Manage the list of frame template for a given document instance.
//			For each document open, there is an associated instance of that
//			class.
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
class CDocFrameMgr
{
    // --------------------------------------------------------------------
    // @group Type definition.
    // --------------------------------------------------------------------
    
	// --------------------------------------------------------------------
    // @group Data definition.
    // --------------------------------------------------------------------
	private:
		// List member.
		FrameList_	m_FrameList;			// The list of frame use by this document.
        CFrame*		m_pFrameToActivate;		// The frame to activate when loading.
		CDocument*	m_pDocument;			// The associated document instance.

	public:
	protected:

    // --------------------------------------------------------------------
    // @group Implementation Method definition.
    // --------------------------------------------------------------------
	private:
        // To find the frame template associated to the given class and frame.
        CFrame* FindFrameEx( CString _ViewClassName, CString _FrameClassName );

	public:
		CDocFrameMgr(	CDocument* _pDocument, 
						FrameTemplateList_& _FrameTemplateList, 
						CFrameTemplate* _pFrameToActivate );
		~CDocFrameMgr( void );

	protected:

    // --------------------------------------------------------------------
    // @group Functional Method definition.
    // --------------------------------------------------------------------
	private:
	public:
		// Usefull method to interact with the different frame.
		bool OpenStartupFrame( void );
		CFrame* GetActiveFrame( void );
		void ShowOrCreateFrame( UINT _nEventID );

		// Search method based on different criteria.
        CFrame* FindFrame( CString _ViewClassName, CString _FrameClassName );
        CFrame* FindFrame( CWnd* _pView, CWnd* _pFrame );   
        CFrame* FindFrame( UINT _nEventId );

		// Method to open the frame associated with the event id.
        BOOL OnCmdMsg(	UINT _nID, int _nCode, void* _pExtra );

		// Wrapper class.
		CDocument* GetDocument( void ) { return m_pDocument; };

	protected:
};

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// @class:	(IMPLEMENTATION_CLASS)
//          That class represent each reel frame that a document may display.
//			Those frame are associated to a document and there is n frame
//			for a document where n is the number of frame template you have
//			added for a document template.
// @base	public | CMultiDocTemplate
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
class CFrame : public CMultiDocTemplate
{
    // --------------------------------------------------------------------
    // @group Friendship definition.
    // --------------------------------------------------------------------

    // --------------------------------------------------------------------
    // @group Macro definition.
    // --------------------------------------------------------------------
    DECLARE_DYNAMIC( CFrame )

    // --------------------------------------------------------------------
    // @group Data definition.
    // --------------------------------------------------------------------
    private:
		CFrameTemplate*	m_pFrameTemplate;		// The frame information.
		CDocument*		m_pDocument;			// The associated document.

    public:
    protected:

    // --------------------------------------------------------------------
    // @group Implementation Method definition.
    // --------------------------------------------------------------------
    private:
    public:
        // @Access Creation and destruction method.
        CFrame( CFrameTemplate* _FrameTemplate, CDocument* _pDocument );
        ~CFrame( void );                

		// Wrapper functions to get object attributes.
		// No need for set functions since everything is provide by the constructor
		// and the only object that has access to tha class is the doc manager.
		CRuntimeClass* GetFrameRTC( void )	  	{ return m_pFrameTemplate->GetFrameRTC(); };
		CRuntimeClass* GetViewRTC( void )		{ return m_pFrameTemplate->GetViewRTC(); };
		UINT GetResourceID( void )		 		{ return m_pFrameTemplate->GetResourceID(); };
		UINT GetEventID( void )		 			{ return m_pFrameTemplate->GetEventID(); };
		UINT GetDefaultWndStatus( void )	 	{ return m_pFrameTemplate->GetDefaultWndStatus(); };
		BOOL GetLoadAtStartup( void )		 	{ return m_pFrameTemplate->GetLoadAtStartup(); };
		CDocument* GetDocument( void )	 	{ return m_pDocument; };

    protected:

    // --------------------------------------------------------------------
    // @group Funcional Method definition.
    // --------------------------------------------------------------------
    private:
    public:
        // Functions used by the framework to manage the window once created.
        CFrameWnd* CreateFrame( void );         // Create the frame window.
        bool DestroyFrame( void );              // Destroy the frame window.
        bool ShowOrCreateFrame( void );         // To display the frame window.
		CFrameWnd* FindFrameWnd( void );		// To find the frame window pointer.
        
    protected:
};

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// @class:  (FUNCTIONAL_CLASS)
//			This class is doing the single document file management.  All
//			the multiple view processing is done in the base class.
// @base    public | CMvDocTemplate
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
class CSDIMVDocTemplate : public CMvDocTemplate
{
    // --------------------------------------------------------------------
    // @group Macro definition.
    // --------------------------------------------------------------------

    // --------------------------------------------------------------------
    // @group Data definition.
    // --------------------------------------------------------------------
    private:
    public:
    protected:
        CDocument* m_pDocument;			// The only document loaded.

    // --------------------------------------------------------------------
    // @group Implementation Method definition.
    // --------------------------------------------------------------------
    private:
    public:
		// Constructor.  Those information are directly passed to the
		// base class that will manage the document creation.
        CSDIMVDocTemplate(	UINT _nIDResource, CRuntimeClass* _pDocClass, 
							BOOL _bAutoDelete = TRUE );
        virtual ~CSDIMVDocTemplate( void );

        // Document related method.	 Those method are needed by the
		// sdi concept.  To support SDI.
    	virtual void AddDocument( CDocument* _pDoc );
        virtual void RemoveDocument( CDocument* _pDoc );
        virtual POSITION GetFirstDocPosition( void ) const;
        virtual CDocument* GetNextDoc( POSITION& _rPos ) const;

    protected:

    // --------------------------------------------------------------------
    // @group Funcional Method definition.
    // --------------------------------------------------------------------
    private:
    public:
    protected:

    // --------------------------------------------------------------------
    // @group MFC Definition.
    // --------------------------------------------------------------------
    private:
    public:
    protected:
};

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// @class:  (FUNCTIONAL_CLASS)
//			This class is doing the multiple document file management.  All
//			the multiple view processing is done in the base class.  No need
//			to do something.  Just use the base mdi stuff.
// @base    public | CMvDocTemplate
// @end =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
class CMDIMVDocTemplate : public CMvDocTemplate
{
    // --------------------------------------------------------------------
    // @group Macro definition.
    // --------------------------------------------------------------------

    // --------------------------------------------------------------------
    // @group Data definition.
    // --------------------------------------------------------------------
    private:
    public:
    protected:

    // --------------------------------------------------------------------
    // @group Implementation Method definition.
    // --------------------------------------------------------------------
    private:
    public:
		// Constructor.  Those information are directly passed to the
		// base class that will manage the document creation.
        CMDIMVDocTemplate(	UINT _nIDResource, CRuntimeClass* _pDocClass, 
							BOOL _bAutoDelete = TRUE );
        virtual ~CMDIMVDocTemplate( void );

    protected:

    // --------------------------------------------------------------------
    // @group Funcional Method definition.
    // --------------------------------------------------------------------
    private:
    public:
        // Document related method.
        virtual void RemoveDocument( CDocument* _pDoc );

    protected:

    // --------------------------------------------------------------------
    // @group MFC Definition.
    // --------------------------------------------------------------------
    private:
    public:
    protected:
};

#endif