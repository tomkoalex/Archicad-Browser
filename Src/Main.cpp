// *****************************************************************************
// Source code for the Browser Control Test Add-On
// *****************************************************************************

// =============================================================================
//
// API Includes
//
// =============================================================================

#include	"APIEnvir.h"
#include	"ACAPinc.h"		// also includes APIdefs.h
#include	"BrowserPalette.hpp"

// -----------------------------------------------------------------------------
// Show or Hide Browser Palette
// -----------------------------------------------------------------------------

static void	ShowOrHideBrowserPalette ()
{
	if (BrowserPalette::HasInstance () && BrowserPalette::GetInstance ().IsVisible ()) {
		BrowserPalette::GetInstance ().Hide ();
	} else {
		if (!BrowserPalette::HasInstance ())
			BrowserPalette::CreateInstance ();
		BrowserPalette::GetInstance ().Show ();
	}
}

// -----------------------------------------------------------------------------
// MenuCommandHandler
//		called to perform the user-asked command
// -----------------------------------------------------------------------------

GSErrCode __ACENV_CALL MenuCommandHandler (const API_MenuParams *menuParams)
{
	switch (menuParams->menuItemRef.menuResID) {
		case BrowserPaletteMenuResId:
			switch (menuParams->menuItemRef.itemIndex) {
				case BrowserPaletteMenuItemIndex:
					ShowOrHideBrowserPalette ();
					break;
			}
			break;
	}

	return NoError;
}


// =============================================================================
//
// Required functions
//
// =============================================================================

// -----------------------------------------------------------------------------
// Dependency definitions
// -----------------------------------------------------------------------------

API_AddonType	__ACDLL_CALL	CheckEnvironment (API_EnvirParams* envir)
{
	RSGetIndString (&envir->addOnInfo.name, 32000, 1, ACAPI_GetOwnResModule ());
	RSGetIndString (&envir->addOnInfo.description, 32000, 2, ACAPI_GetOwnResModule ());

	return APIAddon_Preload;
}		// CheckEnvironment


// -----------------------------------------------------------------------------
// Interface definitions
// -----------------------------------------------------------------------------

GSErrCode	__ACDLL_CALL	RegisterInterface (void)
{
	GSErrCode err = ACAPI_Register_Menu (BrowserPaletteMenuResId, 0, MenuCode_UserDef, MenuFlag_Default);
	if (DBERROR (err != NoError))
		return err;

	return err;
}		// RegisterInterface


// -----------------------------------------------------------------------------
// Initialize
//		called after the Add-On has been loaded into memory
// -----------------------------------------------------------------------------

GSErrCode __ACENV_CALL	Initialize (void)
{
	GSErrCode err = ACAPI_Install_MenuHandler (BrowserPaletteMenuResId, MenuCommandHandler);
	if (DBERROR (err != NoError))
		return err;

	err = ACAPI_Notify_CatchSelectionChange (BrowserPalette::SelectionChangeHandler);
	if (DBERROR (err != NoError))
		return err;

	err = BrowserPalette::RegisterPaletteControlCallBack ();
	if (DBERROR (err != NoError))
		return err;

	return err;
}		// Initialize


// -----------------------------------------------------------------------------
// FreeData
//		called when the Add-On is going to be unloaded
// -----------------------------------------------------------------------------

GSErrCode __ACENV_CALL	FreeData (void)
{
	return NoError;
}		// FreeData
