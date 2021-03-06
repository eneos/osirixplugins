/*=========================================================================
  Program:   OsiriX

  Copyright (c) OsiriX Team
  All rights reserved.
  Distributed under GNU - GPL
  
  See http://www.osirix-viewer.com/copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.
=========================================================================*/

//7/7/05 Fixed bug with DCM Framework and WW and WC. Use float value rather than int value. LP

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#import <Accelerate/Accelerate.h>


#define USEVIMAGE

typedef struct {
   double x,y,z;
} XYZ;

extern XYZ ArbitraryRotate(XYZ p,double theta,XYZ r);

@class xNSImage;
@class ROI;
@class ThickSlabController;
@class DCMObject;
@class Point3D;

/** \brief Represents an image for display */

@interface DCMPix: NSObject <NSCopying>
{
//SOURCES
	NSString            *srcFile;  /**< source File */
	BOOL				isBonjour;  /**< Flag to indicate if file is accessed over Bonour */
	BOOL				nonDICOM;   /**< Flag to indicate if file is not DICOM */

//BUFFERS	
	NSArray				*pixArray;
    NSManagedObject		*imageObj;	/**< Core data object for image */
    xNSImage			*image;    /**< buffer for creating an NSImage */
    short               *oImage;   /**< short buffer of image Data */
	float				*fImage /**< float buffer of image Data */, *fVolImage;  /**< float buffer of volume Data */
    char                *wImage; /**< ? */
	
//DICOM TAGS

//	orientation
	Point3D				*origin;
	double				originX /**< x position of image origin */ , originY /**< y Position of image origin */ , originZ /**< Z position of image origin*/;
	double				orientation[ 9];  /**< pointer to orientation vectors  */

//	pixel representation
	BOOL				fIsSigned;
	short				bitsAllocated, bitsStored, spp;
    float               slope, offset;

//	image size
    long                height, width, rowBytes;

//	window level & width
	float				savedWL, savedWW;

//	planar configuration
	long				fPlanarConf;
    double               pixelSpacingX, pixelSpacingY, pixelRatio;

//	photointerpretation
	BOOL				isRGB;
	BOOL				inverseVal;

//--------------------------------------

// DICOM params needed for SUV calculations
	float				patientsWeight;
	NSString			*repetitiontime;
	NSString			*echotime;
	NSString			*flipAngle, *laterality;
	NSString			*protocolName;
	NSString			*viewPosition;
	NSString			*patientPosition;
	BOOL				hasSUV, SUVConverted;
	NSString			*units, *decayCorrection;
	float				decayFactor;
	float				radionuclideTotalDose;
	float				radionuclideTotalDoseCorrected;
	NSCalendarDate		*acquisitionTime;
	NSCalendarDate		*radiopharmaceuticalStartTime;
	float				halflife;
    float				philipsFactor;
	BOOL				displaySUVValue;

// DICOM params for Overlays - 0x6000 group	
	int					oRows, oColumns, oType, oOrigin[ 2], oBits, oBitPosition;
	unsigned char		*oData;
	
//	DSA-subtraction	
	float				*subtractedfImage;
	NSPoint				subPixOffset;
	NSPoint				subMinMax;
	float				subtractedfPercent;
	float				subtractedfZ;
	float				subtractedfZero;
	float				subtractedfGamma;
	GammaFunction		subGammaFunction;
	
	long				maskID;
	float				maskTime;
	float				fImageTime;
	//float				rot;
	//float				ang;
	NSNumber			*positionerPrimaryAngle;
	NSNumber			*positionerSecondaryAngle;
	
	long				shutterRect_x;
	long				shutterRect_y;
	long				shutterRect_w;
	long				shutterRect_h;
	
	long				shutterCircular_x;
	long				shutterCircular_y;
	long				shutterCircular_radius;
	
	NSPoint	 			*shutterPolygonal;
	long				shutterPolygonalSize;
	
	BOOL				DCMPixShutterOnOff;

//-------------------------------------------------------	
	long				frameNo;
	long				serieNo;
	long				imID, imTot;    
    char                *baseAddr;

//convolution	
	BOOL				convolution, updateToBeApplied;
	short				kernelsize;
	short				normalization;
	short				kernel[25];

	float				cineRate;

//slice
    double				sliceInterval, sliceLocation, sliceThickness;
	double				spacingBetweenSlices;								//SpacingBetweenSlices (0018,0088)
	
//stack
	short				stack;
	short				stackMode, pixPos, stackDirection;
//thickslab
    BOOL				thickSlabVRActivated;
	ThickSlabController *thickSlab;
	
	float				countstackMean;
    float				ww, wl;
	float				fullww, fullwl;
	BOOL				fixed8bitsWLWW;	
    float               maxValueOfSeries, minValueOfSeries;
	
	
	NSString			*convertedDICOM;	
	BOOL				generated;	
	NSLock				*checking;
	NSLock				*processorsLock;
	volatile int		numberOfThreadsForCompute;
	
	BOOL				useVOILUT;
	int					VOILUT_first;
	unsigned int		VOILUT_number, VOILUT_depth, *VOILUT_table;
	
	char				blackIndex;
	
	NSData				*transferFunction;
	float				*transferFunctionPtr;
	
/** custom annotations */
	NSMutableDictionary *annotationsDictionary;
	NSMutableDictionary *cachedPapyGroups;
}

@property long frameNo;
@property(setter=setID:) long ID;

@property float minValueOfSeries, maxValueOfSeries;

// Dimensions in pixels
@property long pwidth, pheight;

/** Is it an RGB image (ARGB) or float image?
Note setter is different to not break existing usage. :-( */
@property(setter=setRGB:) BOOL isRGB;  

/** Pointer to image data */
@property(setter=setfImage:) float* fImage;

/** WW & WL */
@property(readonly) float ww, wl, fullww, fullwl;
@property float savedWW, savedWL;

@property(readonly) float slope, offset;

/**  X/Y ratio - non-square pixels */
@property double pixelRatio;

/**  pixel size */
@property double pixelSpacingX, pixelSpacingY;

/** Slice orientation */
- (void)orientation:(float*) c;
- (void)setOrientation:(float*) c;

- (void)orientationDouble:(double*) c;
- (void)setOrientationDouble:(double*) c;

/** Slice location */
@property(readonly) double originX, originY, originZ;

- (void)setOrigin :(float*) o;
- (void)setOriginDouble :(double*) o;

/**  Axial Location */
@property double sliceLocation;
/**  Slice Thickness */
@property double sliceThickness;
/**  Slice Interval */
@property double sliceInterval;
/**  Gap between slices */
@property(readonly) double spacingBetweenSlices;

/**  8-bit TransferFunction */
@property(retain) NSData *transferFunction; 

@property NSPoint subPixOffset;

@property long DCMPixShutterRectWidth, DCMPixShutterRectHeight;
@property long DCMPixShutterRectOriginX, DCMPixShutterRectOriginY;

@property(retain) NSString *repetitiontime, *echotime;
@property(readonly) NSString *flipAngle, *laterality;

@property(readonly) NSString *protocolName;
@property(readonly) NSString *viewPosition;
@property(readonly) NSString *patientPosition;

@property char* baseAddr;
@property unsigned char* LUT12baseAddr;

@property long rowBytes;
@property(readonly) long serieNo;

@property(getter=Tot, setter=setTot:) long Tot;

@property(readonly) short stack, stackMode;
@property(readonly) BOOL generated;
@property(retain) NSString *sourceFile;

/** Database links */
@property(readonly) NSManagedObject *imageObj, *seriesObj;
@property(readonly) NSString *srcFile;
@property(readonly) NSMutableDictionary *annotationsDictionary;

// Properties (aka accessors) needed for SUV calculations
@property(readonly) float philipsFactor;
@property float patientsWeight;
@property float halflife;
@property float radionuclideTotalDose;
@property float radionuclideTotalDoseCorrected;
@property(retain) NSCalendarDate *acquisitionTime;
@property(retain) NSCalendarDate *radiopharmaceuticalStartTime;
@property BOOL SUVConverted;
@property(readonly) BOOL hasSUV;
@property float decayFactor;
@property(retain) NSString *units, *decayCorrection;
@property BOOL displaySUVValue;

- (void) copySUVfrom: (DCMPix*) from;  /**< Copy the SUV from another DCMPic */
- (float) getPixelValueX: (long) x Y:(long) y;  /**< Get the pixel for a point with x,y coordinates */

- (void) checkSUV; /**< Makes sure all the necessary values for SUV calculation are present */

+ (void) checkUserDefaults: (BOOL) update;  /**< Check User Default for needed setting */
+ (void) resetUserDefaults;  /**< Reset the defaults */
 /** Determine if a point is inside a polygon
 * @param x is the NSPoint to check. 
 * @param  poly is a pointer to an array of NSPoints. 
 * @param count is the number of 
 * points in the polygon.
*/
+ (BOOL) IsPoint:(NSPoint) x inPolygon:(NSPoint*) poly size:(int) count; 


- (void) changeWLWW:(float)newWL :(float)newWW;  /**< Change window level to window width to the new values */
- (void) computePixMinPixMax;  /**< Compute the min and max values in the image */

// Compute ROI data
/** Calculates the cofactor used Calcium scoring.  
* Depends on the threshold used for scoring 
* Threshold is usually 90 or 120 depending on whether the source is
* Electron Beam or Multislice CT
*/
- (int)calciumCofactorForROI:(ROI *)roi threshold:(int)threshold;  

/** returns calculated values for ROI:
*  mean, total, deviation, min, max
*/
- (void) computeROI:(ROI*) roi :(float *)mean :(float *)total :(float *)dev :(float *)min :(float *)max;  
/** Not sure when this is used rather than computeROI: */
- (void) computeROIInt:(ROI*) roi :(float*) mean :(float *)total :(float *)dev :(float *)min :(float *)max;

/** Fill a ROI with a value
* @param roi  Selected ROI
* @param newVal  The replacement value
* @param minValue  Lower threshold
* @param maxValue Upper threshold
* @param outside  if YES replace outside the ROI
* @param orientationStack  
* @param stackNo  
* @param restore  
* @param addition  
*/
- (void) fillROI:(ROI*) roi newVal:(float) newVal minValue:(float) minValue maxValue:(float) maxValue outside:(BOOL) outside orientationStack:(long) orientationStack stackNo:(long) stackNo restore:(BOOL) restore addition:(BOOL) addition;
- (void) fillROI:(ROI*) roi newVal:(float) newVal minValue:(float) minValue maxValue:(float) maxValue outside:(BOOL) outside orientationStack:(long) orientationStack stackNo:(long) stackNo restore:(BOOL) restore addition:(BOOL) addition spline:(BOOL) spline;

/** Fill a ROI with a value
* @param roi  Selected ROI
* @param newVal  The replacement value
* @param minValue  lower threshold
* @param maxValue  upper threshold
* @param outside  if YES replace outside the ROI
* @param orientationStack  ?
* @param  stackNo  
* @param  restore  
*/
- (void) fillROI:(ROI*) roi :(float) newVal :(float) minValue :(float) maxValue :(BOOL) outside :(long) orientationStack :(long) stackNo :(BOOL) restore;

/** Fill a ROI with a value
* @param roi  Selected ROI
* @param newVal  The replacement value
* @param minValue  lower threshold
* @param maxValue  upper threshold
* @param outside  if YES replace outside the ROI
* @param orientation  
* @param stackNo   
*/
- (void) fillROI:(ROI*) roi :(float) newVal :(float) minValue :(float) maxValue :(BOOL) outside :(long) orientation :(long) stackNo;

/** Fill a ROI with a value.
* @param roi Selected ROI
* @param newVal  The replacement value
* @param minValue  Lower threshold
* @param maxValue  Upper threshold
* @param outside  If YES replace outside the ROI
*/
- (void) fillROI:(ROI*) roi :(float) newVal :(float) minValue :(float) maxValue :(BOOL) outside;

- (unsigned char*) getMapFromPolygonROI:(ROI*) roi size:(NSSize*) size origin:(NSPoint*) origin; /**< Map from Polygon ROI */

/** Is this Point (pt) in this ROI ? */
- (BOOL) isInROI:(ROI*) roi :(NSPoint) pt;

/** Returns a pointer with all pixels values contained in the current ROI
* User must Free the pointer with the free() function
* Returns reference number of pixels in numberOfValues
* Returns a pointer to the pixel locations. Each point has the x position followed by the y position
* Locations is malloced but not freed
*/
- (float*) getROIValue :(long*) numberOfValues :(ROI*) roi :(float**) locations;

/** Returns a pointer with all pixels values contained in the current ROI
* User must Free the pointer with the free() function
* Returns reference number of pixels in numberOfValues
* Returns a pointer to the pixel locations. Each point has the x position followed by the y position
* Locations is malloced but not freed
*/
- (float*) getLineROIValue :(long*) numberOfValues :(ROI*) roi;


/** Utility methods to convert user supplied pixel coords to DICOM patient coords float d[3] (in mm)
* using current slice location and orientation
*/
-(void) convertPixX: (float) x pixY: (float) y toDICOMCoords: (float*) d;

/** Utility methods to convert user supplied pixel coords to DICOM patient coords double d[3] (in mm)
* using current slice location and orientation
*/
-(void) convertPixDoubleX: (double) x pixY: (double) y toDICOMCoords: (double*) d;

/** convert DICOM coordinated to slice coordinates */
-(void) convertDICOMCoords: (float*) dc toSliceCoords: (float*) sc;

/** convert DICOM coordinated to slice coordinates */
-(void) convertDICOMCoordsDouble: (double*) dc toSliceCoords: (double*) sc;


/** Return index & sliceCoords */
+(int) nearestSliceInPixelList: (NSArray*)pixlist withDICOMCoords: (float*)dc sliceCoords: (float*) sc;  



- (BOOL) thickSlabVRActivated; /**< Activate Thick Slab VR */

/** convert to Black and White. 
* @param mode values: 0 Use Red Channel, 1 use Green Channel 2 use Blue Channel  3 Merge and use RGB
*/
- (void) ConvertToBW:(long) mode; 

/** convert to RGB. 
* @param mode values: 0 create Red Channel, 1 create Green Channel 2 create Blue Channel  3 create all channels
* @param  cwl  = window level to use
* @param cww = window width to use
*/
- (void) ConvertToRGB:(long) mode :(long) cwl :(long) cww;

- (float) cineRate;  /**< Returns the Cine rate */

// drag-drop subtraction-multiplication between series
- (void) imageArithmeticMultiplication:(DCMPix*) sub;
- (float*) multiplyImages :(float*) input :(float*) subfImage;
- (void) imageArithmeticSubtraction:(DCMPix*) sub;
- (float*) arithmeticSubtractImages :(float*) input :(float*) subfImage;

//DSA
- (void) setSubSlidersPercent: (float) p gamma: (float) g zero: (float) z;
- (void) setSubSlidersPercent: (float) p;
- (NSPoint) subMinMax:(float*)input :(float*)subfImage;
- (void) setSubtractedfImage:(float*)mask :(NSPoint)smm;
- (float*) subtractImages:(float*)input :(float*)subfImage;

-(void) fImageTime:(float)newTime;
-(float) fImageTime;
-(void) maskID:(long)newID;
-(long) maskID;
-(void) maskTime:(float)newMaskTime;
-(float) maskTime;
-(void) positionerPrimaryAngle:(NSNumber *)newPositionerPrimaryAngle;
-(NSNumber*) positionerPrimaryAngle;
-(void) positionerSecondaryAngle:(NSNumber*)newPositionerSecondaryAngle;
-(NSNumber*) positionerSecondaryAngle;
+ (NSPoint) originDeltaBetween:(DCMPix*) pix1 And:(DCMPix*) pix2;
- (void) setBlackIndex:(int) i;
+ (NSImage*) resizeIfNecessary:(NSImage*) currentImage dcmPix: (DCMPix*) dcmPix;
-(void) DCMPixShutterRect:(long)x:(long)y:(long)w:(long)h;
-(BOOL) DCMPixShutterOnOff;
-(void) DCMPixShutterOnOff:(BOOL)newDCMPixShutterOnOff;
- (void) computeTotalDoseCorrected;
//- (void) copyFromOther:(DCMPix *) fromDcm;
- (void) imageArithmeticMultiplication:(DCMPix*) sub;
- (void) setRGB : (BOOL) val;
- (void) setConvolutionKernel:(short*)val :(short) size :(short) norm;
- (void) applyConvolutionOnSourceImage;
- (void) setArrayPix :(NSArray*) array :(short) i;
- (BOOL) updateToApply;
- (id) myinitEmpty;  /**< Returns an Empty object */
- (short*) kernel;
- (short) normalization;
- (short) kernelsize;

/**  calls 
* myinit:(NSString*) s :(long) pos :(long) tot :(float*) ptr :(long) f :(long) ss isBonjour:(BOOL) hello imageObj: (NSManagedObject*) iO
* with hello = NO and iO = 0L
*/
- (id) myinit:(NSString*) s :(long) pos :(long) tot :(float*) ptr :(long) f :(long) ss;

/**  Initialize
* doesn't load pix data, only initializes instance variables
* @param s  filename
* @param pos  imageID  Position in array.
* @param tot  imTot  Total number of images. 
* @param ptr  pointer to volume
* @param f  frame number
* @param ss  series number
* @param hello  flag to indicate remote bonjour file
* @param iO  coreData image Entity for image
*/ 
- (id) myinit:(NSString*) s :(long) pos :(long) tot :(float*) ptr :(long) f :(long) ss isBonjour:(BOOL) hello imageObj: (NSManagedObject*) iO;

/** init with data pointer
* @param im  pointer to image data
* @param pixelSize  pixelDepth in bits
* @param xDim  image width
* @param yDim =image height
* @param xSpace  pixel width
* @param ySpace  pxiel height
* @param oX x position of origin
* @param oY y position of origin
* @param oZ z position of origin
*/
- (id) initwithdata :(float*) im :(short) pixelSize :(long) xDim :(long) yDim :(float) xSpace :(float) ySpace :(float) oX :(float) oY :(float) oZ;

/** init with data pointer
* @param im = pointer to image data
* @param pixelSize = pixelDepth in bits
* @param xDim  image width
* @param yDim  image height
* @param xSpace  pixel width
* @param ySpace  pxiel height
* @param oX x position of origin
* @param oY y position of origin
* @param oZ z position of origin
* @param volSize ?
*/
- (id) initwithdata :(float*) im :(short) pixelSize :(long) xDim :(long) yDim :(float) xSpace :(float) ySpace :(float) oX :(float) oY :(float) oZ :(BOOL) volSize;
- (id) initWithImageObj: (NSManagedObject *)entity;
- (id) initWithContentsOfFile: (NSString *)file; 
/** create an NSImage from the current pix
* @param smallIcon  thumbnail size
* @param newWW  window width to use
* @param newWL window level to use;
*/
- (NSImage*) computeWImage: (BOOL) smallIcon :(float)newWW :(float)newWL;

/** create an NSImage from the current pix using the current ww/wl. Full size*/
- (NSImage*) image;

/** reeturns the current image. returns nil if no image has be previously created */
- (NSImage*) getImage;

/** A pointer to the orientation.  9 values in length. 3 for each axis. */
- (void) orientation:(float*) c;

/** Sets the orientation.  9 values in length. 3 for each axis. */
- (void) setOrientation:(float*) c;

/** A short pointer to the image data */
- (short*) oImage;

/** Releases the current NSImage */
- (void) kill8bitsImage;


/** if no image. Creates one using
* computeWImage: (BOOL) smallIcon :(float)newWW :(float)newWL
*/ 
- (void) checkImageAvailble:(float)newWW :(float)newWL;

/** Load the DICOM image using the DCMFramework.  
* There should be no reason to call this. The class will call it when needed. */
- (BOOL)loadDICOMDCMFramework;

/** Load the DICOM image using Papyrus.
* There should be no reason to call this. The class will call it when needed.
*/
- (BOOL) loadDICOMPapyrus;

/** Reset the Annotations */
- (void) reloadAnnotations;


/** Parses the file. Extracts necessary data. Load image data.
* This class will be called by the class when necessay. 
* There should be no need to call it externally
*/
- (void) CheckLoadIn;

/** Calls CheckLoadIn when needed */
- (void) CheckLoad;

/** Compute the float pointer for the image data */
- (float*) computefImage;

/** Sets fusion paramaters
* @param m  stack mode
* @param s stack
* @param direction stack direction
*/
-(void) setFusion:(short) m :(short) s :(short) direction;

/** Sets updateToBeApplied to YES. It is called whenver a setting has been changed.  
* Should be called by the class automatically when needed */
-(void) setUpdateToApply;


/** Releases the fImage and sets all values to nil. */
- (void)revert;

/** finds the min and max pixel values. 
* Sets the appropriate values for fullWW and fullWL 
*/
- (void) computePixMinPixMax;

/** Sets the ThickSlabController */
- (void) setThickSlabController:( ThickSlabController*) ts;


/** Sets the fixed8bitsWLWW flag */
- (void) setFixed8bitsWLWW:(BOOL) f;

/** Creates a DCMPix with the original values and places it in the restore cache*/
- (void) prepareRestore;


/** Releases the restored DCMPix from the restoreCache */
- (void) freeRestore;

/** Sets flag for when OsiriX is running in protected mode */
+ (void) setRunOsiriXInProtectedMode:(BOOL) v;

/** Returns flag for protected mode */
+ (BOOL) isRunOsiriXInProtectedModeActivated;

/** Clears the papyrus group cache */
- (void) clearCachedPapyGroups;

/** Returns a pointer the the papyrus group
* @param group group
* @param fileNb papyrus file
*/
- (void *) getPapyGroup: (int)group fileNb: (int)fileNb;

/** create ROIs from RTSTRUCT */
- (void)createROIsFromRTSTRUCT: (DCMObject*)dcmObject;

#ifdef OSIRIX_VIEWER

/** Custom Annotations */
- (void)loadCustomImageAnnotationsPapyLink:(int)fileNb DCMLink:(DCMObject*)dcmObject;


/** Set flag to anonymize the annotations */
+ (BOOL) setAnonymizedAnnotations: (BOOL) v;
#endif

@end
