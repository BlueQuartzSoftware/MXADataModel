#ifndef _MXA_H5IMAGE_H
#define _MXA_H5IMAGE_H

//MXA Includes
#include "Headers/DLLExport.h"

//-- STL headers
#include <string>

//-- HDF5 Header
#include "hdf5.h"

namespace H5ImageConst
{
  // -----------------------------------------------------------------------------
  //  
  // -----------------------------------------------------------------------------
    const std::string ImageClass ("CLASS");
    const std::string Image("IMAGE");
    const std::string ImageVersion("IMAGE_VERSION");
    const std::string ImageSubclass("IMAGE_SUBCLASS");
    const std::string ImageIndexed("IMAGE_INDEXED");
    const std::string InterlacePixel("INTERLACE_PIXEL");
    const std::string InterlacePlane("INTERLACE_PLANE");
    const std::string ImageTrueColor("IMAGE_TRUECOLOR");
    const std::string InterlaceMode ("INTERLACE_MODE");
    const std::string Palette("PALETTE");
    const std::string PalVersion("PAL_VERSION");
    const std::string PalColorMap("PAL_COLORMAP");
    const std::string PalType("PAL_TYPE");
    const std::string Standard8("STANDARD8");
    const std::string RGB("RGB");
    const std::string PalVersionValue("1.2");
    const std::string ImageGrayScale("IMAGE_GRAYSCALE");
    const std::string ImageWhiteIsZero("IMAGE_WHITE_IS_ZERO");
    const std::string ImageColorModel("IMAGE_COLORMODEL");
    const std::string DisplayOrigin("DISPLAY_ORIGIN");
    const std::string UpperLeft("UL");
    const std::string LowerLeft("LL");
    const std::string UpperRight("UR");
    const std::string LowerRight("LR");
}


class H5Image
{

public:


static MXA_EXPORT herr_t H5IM_find_palette( hid_t loc_id );

static MXA_EXPORT herr_t H5IMmake_image_8bit( hid_t loc_id,
                            std::string datasetName,
                            hsize_t width,
                            hsize_t height,
                            const unsigned char *buffer );

static MXA_EXPORT herr_t H5IMmake_image_24bit( hid_t loc_id,
                             std::string datasetName,
                             hsize_t width,
                             hsize_t height,
                             const std::string &interlace,
                             const unsigned char *buffer );

static MXA_EXPORT herr_t H5IMget_image_info( hid_t loc_id,
                     std::string datasetName,
                     hsize_t *width,
                     hsize_t *height,
                     hsize_t *planes,
                     std::string interlace,
                     hssize_t *npals );

static MXA_EXPORT herr_t H5IMread_image( hid_t loc_id,
                       std::string datasetName,
                       unsigned char *buffer );



static MXA_EXPORT herr_t H5IMmake_palette( hid_t loc_id,
                         std::string pal_name,
                         const hsize_t *pal_dims,
                         const unsigned char *pal_data );

static MXA_EXPORT herr_t H5IMlink_palette( hid_t loc_id,
                        std::string imageName,
                        std::string pal_name );

static MXA_EXPORT herr_t H5IMunlink_palette( hid_t loc_id,
                           std::string imageName,
                           std::string pal_name );

static MXA_EXPORT herr_t H5IMget_npalettes( hid_t loc_id,
                          std::string imageName,
                          hssize_t *npals );

static MXA_EXPORT herr_t H5IMget_palette_info( hid_t loc_id,
                        std::string imageName,
                        int pal_number,
                        hsize_t *pal_dims );


static MXA_EXPORT herr_t H5IMget_palette( hid_t loc_id,
                        std::string imageName,
                        int pal_number,
                        unsigned char *pal_data );

static MXA_EXPORT herr_t H5IMis_image( hid_t loc_id,
                     std::string datasetName );

static MXA_EXPORT herr_t H5IMis_palette( hid_t loc_id,
                     std::string datasetName );

};


/*-------------------------------------------------------------------------
 * Function: find_palette
 *
 * Purpose: operator function used by H5LT_find_palette
 *
 * Return:
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: May 28, 2001
 *
 * Comments:
 *
 * Modifications:
 *
 *-------------------------------------------------------------------------
 */
static herr_t find_palette( hid_t loc_id, const char *name, void  *op_data )
{

 /* Define a default zero value for return. This will cause the iterator to continue if
  * the palette attribute is not found yet.
  */

 int ret = 0;

 /* Shut compiler */
 loc_id=loc_id;
 op_data=op_data;

 /* Define a positive value for return value if the attribute was found. This will
  * cause the iterator to immediately return that positive value,
  * indicating short-circuit success
  */

 if( strcmp( name, "PALETTE" ) == 0 )
  ret = 1;


 return ret;
}




#endif
