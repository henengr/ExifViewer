#pragma once

#include <cstdint>
#include <vector>
#include <string>


enum class enIFDType: std::uint16_t {
    BYTE = 1,           // 8-bit unsigned integer
    ASCII,              // ASCII character
    SHORT,              // 16-bit unsigned integer
    LONG,               // 32-bit unsigned integer
    RATIONAL,           // 2 of 16-bit unsigned integer - The first value represents the numerator, and the second value represents the denominator.
    UNDEFINED = 7,      // undedined 8-bit value
    SLONG = 9,          // 32-bit signed integer
    SRATIONAL,          // 2 of 16-bit signed integer - The first value represents the numerator, and the second value represents the denominator.
    UTF8 = 129,         // UTF-8
};

struct stIFDTag {
    std::uint16_t       tag;
    std::uint16_t       type;
    std::uint32_t       count;
    std::uint32_t       offset;
    const char*         description;
};


constexpr std::uint16_t to_u16(enIFDType t){
    return static_cast<std::uint16_t>(t);
}

constexpr std::uint16_t IFDTAG_ExifIFDPointer = 0x8769;
constexpr std::uint16_t IFDTAG_GPSInfoIFDPointer = 0x8825;

constexpr stIFDTag IFDTable[] = {
    // Tags related to image composition
    {0x8769, to_u16(enIFDType::LONG),       1,  0,      "Exif IFD Pointer"},
    {0x8825, to_u16(enIFDType::LONG),       1,  0,      "GPS Info IFD Pointer"},
    {0xa005, to_u16(enIFDType::LONG),       1,  0,      "Interoperability IFD Pointer"},
    // TIFF tags used in Exif
    {0x0100, to_u16(enIFDType::SHORT),      1,  0,      "ImageWidth"},
    {0x0100, to_u16(enIFDType::LONG),       1,  0,      "ImageWidth"},
    {0x0101, to_u16(enIFDType::SHORT),      1,  0,      "ImageHeight"},
    {0x0101, to_u16(enIFDType::LONG),       1,  0,      "ImageHeight"},
    {0x0102, to_u16(enIFDType::SHORT),      3,  0,      "BitsPerSample"},
    {0x0103, to_u16(enIFDType::SHORT),      1,  0,      "Compression"},
    {0x0106, to_u16(enIFDType::SHORT),      1,  0,      "PhotometricInterpretation"},
    {0x0112, to_u16(enIFDType::SHORT),      1,  0,      "Orientation"},
    {0x0115, to_u16(enIFDType::SHORT),      1,  0,      "SamplesPerPixel"},
    {0x011c, to_u16(enIFDType::SHORT),      1,  0,      "PlannerConfiguration"},
    {0x0212, to_u16(enIFDType::SHORT),      2,  0,      "YCbCrSubSampling"},
    {0x0213, to_u16(enIFDType::SHORT),      1,  0,      "YCbCrPositioning"},
    {0x011a, to_u16(enIFDType::RATIONAL),   1,  0,      "XResolution"},
    {0x011b, to_u16(enIFDType::RATIONAL),   1,  0,      "YResolution"},
    {0x0128, to_u16(enIFDType::SHORT),      1,  0,      "ResolutionUnit"},
    {0x0111, to_u16(enIFDType::SHORT),      1,  0,      "StripOffsets"},
    {0x0111, to_u16(enIFDType::LONG),       1,  0,      "StripOffsets"},
    {0x0116, to_u16(enIFDType::SHORT),      1,  0,      "RowsPerStrip"},
    {0x0116, to_u16(enIFDType::LONG),       1,  0,      "RowsPerStrip"},
    {0x0117, to_u16(enIFDType::SHORT),      1,  0,      "StripByteCounts"},
    {0x0117, to_u16(enIFDType::LONG),       1,  0,      "StripByteCounts"},
    {0x0201, to_u16(enIFDType::LONG),       1,  0,      "JPEGInterchangeFormat"},
    {0x0202, to_u16(enIFDType::LONG),       1,  0,      "JPEGInterchangeFormatLength"},
    {0x012d, to_u16(enIFDType::SHORT),3 * 256,  0,      "TransferFunction"},
    {0x013e, to_u16(enIFDType::RATIONAL),   2,  0,      "WhitePoint"},
    {0x013f, to_u16(enIFDType::RATIONAL),   6,  0,      "PrimaryChromaticities"},
    {0x0211, to_u16(enIFDType::RATIONAL),   3,  0,      "YCbCrCoefficients"},
    {0x0214, to_u16(enIFDType::RATIONAL),   6,  0,      "ReferenceBlackWhite"},
    {0x0132, to_u16(enIFDType::ASCII),     20,  0,      "DateTime"},
    //{0x010e, to_u16(enIFDType::ASCII), 0xffff,  0,      "ImageDescription"},
    //{0x010f, to_u16(enIFDType::ASCII), 0xffff,  0,      "Make"},
    //{0x0110, to_u16(enIFDType::ASCII), 0xffff,  0,      "Model"},
    //{0x0131, to_u16(enIFDType::ASCII), 0xffff,  0,      "Software"},
    //{0x013b, to_u16(enIFDType::ASCII), 0xffff,  0,      "Artist"},
    {0x8298, to_u16(enIFDType::ASCII), 0xffff,  0,      "Copyright"},
    // Exif IFDs
    {0x9000, to_u16(enIFDType::UNDEFINED),  4,  0,      "ExifVersion"},
    {0xa000, to_u16(enIFDType::UNDEFINED),  4,  0,      "FlashpixVersion"},
    {0xa001, to_u16(enIFDType::SHORT),      1,  0,      "ColorSpace"},
    {0xa500, to_u16(enIFDType::RATIONAL),   1,  0,      "Gamma"},
    {0x9101, to_u16(enIFDType::UNDEFINED),  4,  0,      "ComponentsConfiguration"},
    {0x9102, to_u16(enIFDType::RATIONAL),   1,  0,      "CompressedBitsPerPixel"},
    {0xa002, to_u16(enIFDType::SHORT),      1,  0,      "PixelXDimension"},
    {0xa002, to_u16(enIFDType::LONG),       1,  0,      "PixelXDimension"},
    {0xa003, to_u16(enIFDType::SHORT),      1,  0,      "PixelYDimension"},
    {0xa003, to_u16(enIFDType::LONG),       1,  0,      "PixelYDimension"},
    {0x927c, to_u16(enIFDType::UNDEFINED),0xffff, 0,    "MakerNote"},
    {0x9286, to_u16(enIFDType::UNDEFINED),0xffff, 0,    "UserComment"},
    {0xa004, to_u16(enIFDType::ASCII),     13,  0,      "RelatedSoundFile"},
    {0x9003, to_u16(enIFDType::ASCII),     20,  0,      "DateTimeOriginal"},
    {0x9004, to_u16(enIFDType::ASCII),     20,  0,      "DateTimeDigitized"},
    {0x9290, to_u16(enIFDType::ASCII), 0xffff,  0,      "SubSecTime"},
    {0x9291, to_u16(enIFDType::ASCII), 0xffff,  0,      "SubSecTimeOriginal"},
    {0x9292, to_u16(enIFDType::ASCII), 0xffff,  0,      "SubSecTimeDigitized"},
    {0x829a, to_u16(enIFDType::RATIONAL),   1,  0,      "ExposureTime"},
    {0x829d, to_u16(enIFDType::RATIONAL),   1,  0,      "FNumber"},
    {0x8822, to_u16(enIFDType::SHORT),      1,  0,      "ExposureProgram"},
    {0x8824, to_u16(enIFDType::ASCII), 0xffff,  0,      "SpectralSensitivity"},
    {0x8827, to_u16(enIFDType::SHORT), 0xffff,  0,      "PhotographicSensitivity"},
    {0x8828, to_u16(enIFDType::UNDEFINED),0xffff, 0,    "OECF"},
    {0x8830, to_u16(enIFDType::SHORT),      1,  0,      "SensitivityType"},
    {0x8831, to_u16(enIFDType::LONG),       1,  0,      "StandardOutputSensitivity"},
    {0x8832, to_u16(enIFDType::LONG),       1,  0,      "RecommendedExposureIndex"},
    {0x8833, to_u16(enIFDType::LONG),       1,  0,      "ISOSpeed"},
    {0x8834, to_u16(enIFDType::LONG),       1,  0,      "ISOSpeedLatitudeyyy"},
    {0x8835, to_u16(enIFDType::LONG),       1,  0,      "ISOSpeedLatitudezzz"},
    {0x9201, to_u16(enIFDType::SRATIONAL),  1,  0,      "ShutterSpeedValue"},
    {0x9202, to_u16(enIFDType::RATIONAL),   1,  0,      "ApertureValue"},
    {0x9203, to_u16(enIFDType::SRATIONAL),  1,  0,      "BrightnessValue"},
    {0x9204, to_u16(enIFDType::SRATIONAL),  1,  0,      "ExposureBiasValue"},
    {0x9205, to_u16(enIFDType::RATIONAL),   1,  0,      "MaxApertureValue"},
    {0x9206, to_u16(enIFDType::RATIONAL),   1,  0,      "SubjectDistance"},
    {0x9207, to_u16(enIFDType::SHORT),      1,  0,      "MeteringMode"},
    {0x9208, to_u16(enIFDType::SHORT),      1,  0,      "LightSource"},
    {0x9209, to_u16(enIFDType::SHORT),      1,  0,      "Flash"},
    {0x920a, to_u16(enIFDType::RATIONAL),   1,  0,      "FocalLength"},
    {0x9214, to_u16(enIFDType::SHORT), 0xffff,  0,      "SubjectArea"}, // カウント 2 or 3 or 4 TBD
    {0xa20b, to_u16(enIFDType::RATIONAL),   1,  0,      "FlashEnergy"},
    {0xa20c, to_u16(enIFDType::UNDEFINED),0xffff, 0,    "SpatialFrequencyResponse"},
    {0xa20e, to_u16(enIFDType::RATIONAL),   1,  0,      "FocalPlaneXResolution"},
    {0xa20f, to_u16(enIFDType::RATIONAL),   1,  0,      "FocalPlaneYResolution"},
    {0xa210, to_u16(enIFDType::SHORT),      1,  0,      "FocalPlaneResolutionUnit"},
    {0xa214, to_u16(enIFDType::SHORT),      2,  0,      "SubjectLocation"},
    {0xa215, to_u16(enIFDType::RATIONAL),   1,  0,      "ExposureIndex"},
    {0xa217, to_u16(enIFDType::SHORT),      1,  0,      "SensingMethod"},
    {0xa300, to_u16(enIFDType::UNDEFINED),  1,  0,      "FileSource"},
    {0xa301, to_u16(enIFDType::UNDEFINED),  1,  0,      "SceneType"},
    {0xa302, to_u16(enIFDType::UNDEFINED),0xffff, 0,    "CFAPattern"},
    {0xa401, to_u16(enIFDType::SHORT),      1,  0,      "CustomRendered"},
    {0xa402, to_u16(enIFDType::SHORT),      1,  0,      "ExposureMode"},
    {0xa403, to_u16(enIFDType::SHORT),      1,  0,      "WhiteBalance"},
    {0xa404, to_u16(enIFDType::RATIONAL),   1,  0,      "DigitalZoomRatio"},
    {0xa405, to_u16(enIFDType::SHORT),      1,  0,      "FocalLengthIn35mmFilm"},
    {0xa406, to_u16(enIFDType::SHORT),      1,  0,      "SceneCaptureType"},
    {0xa407, to_u16(enIFDType::SHORT),      1,  0,      "GainControl"},
    {0xa408, to_u16(enIFDType::SHORT),      1,  0,      "Contrast"},
    {0xa409, to_u16(enIFDType::SHORT),      1,  0,      "Saturation"},
    {0xa40a, to_u16(enIFDType::SHORT),      1,  0,      "Sharpness"},
    {0xa40b, to_u16(enIFDType::UNDEFINED),0xffff, 0,    "DeviceSettingDescription"},
    {0xa40c, to_u16(enIFDType::SHORT),      1,  0,      "SubjectDistanceRange"},
    {0xa420, to_u16(enIFDType::ASCII),     33,  0,      "ImageUniqueID"},
    {0xa430, to_u16(enIFDType::ASCII), 0xffff,  0,      "CameraOwnerName"},
    {0xa431, to_u16(enIFDType::ASCII), 0xffff,  0,      "BodySerialNumber"},
    {0xa432, to_u16(enIFDType::RATIONAL),   4,  0,      "LensSpecification"},
    {0xa433, to_u16(enIFDType::ASCII), 0xffff,  0,      "LensMake"},
    {0xa434, to_u16(enIFDType::ASCII), 0xffff,  0,      "LensModel"},
    {0xa435, to_u16(enIFDType::ASCII), 0xffff,  0,      "LensSerialNumber"},
    // GPS IFDs
    {0x0000, to_u16(enIFDType::BYTE),       4,  0,      "GPSVersionID"},
    {0x0001, to_u16(enIFDType::ASCII),      2,  0,      "GPSLatitudeRef"},
    {0x0002, to_u16(enIFDType::RATIONAL),   3,  0,      "GPSLatitude"},
    {0x0003, to_u16(enIFDType::ASCII),      2,  0,      "GPSLongitudeRef"},
    {0x0004, to_u16(enIFDType::RATIONAL),   3,  0,      "GPSLongitude"},
    {0x0005, to_u16(enIFDType::BYTE),       1,  0,      "GPSAltitudeRef"},
    {0x0006, to_u16(enIFDType::RATIONAL),   1,  0,      "GPSAltitude"},
    {0x0007, to_u16(enIFDType::RATIONAL),   3,  0,      "GPSTimeStamp"},
    {0x0008, to_u16(enIFDType::ASCII), 0xffff,  0,      "GPSSatellites"},
    {0x0009, to_u16(enIFDType::ASCII),      2,  0,      "GPSStatus"},
    {0x000a, to_u16(enIFDType::ASCII),      2,  0,      "GPSMeasureMode"},
    {0x000b, to_u16(enIFDType::RATIONAL),   1,  0,      "GPSDOP"},
    {0x000c, to_u16(enIFDType::ASCII),      2,  0,      "GPSSpeedRef"},
    {0x000d, to_u16(enIFDType::RATIONAL),   1,  0,      "GPSSpeed"},
    {0x000e, to_u16(enIFDType::ASCII),      2,  0,      "GPSTrackRef"},
    {0x000f, to_u16(enIFDType::RATIONAL),   1,  0,      "GPSTrack"},
    {0x0010, to_u16(enIFDType::ASCII),      2,  0,      "GPSImgDirectionRef"},
    {0x0011, to_u16(enIFDType::RATIONAL),   1,  0,      "GPSImgDirection"},
    {0x0012, to_u16(enIFDType::ASCII), 0xffff,  0,      "GPSMapDatum"},
    {0x0013, to_u16(enIFDType::ASCII),      2,  0,      "GPSDestLatitudeRef"},
    {0x0014, to_u16(enIFDType::RATIONAL),   3,  0,      "GPSDestLatitude"},
    {0x0015, to_u16(enIFDType::ASCII),      2,  0,      "GPSDestLongitudeRef"},
    {0x0016, to_u16(enIFDType::RATIONAL),   3,  0,      "GPSDestLongitude"},
    {0x0017, to_u16(enIFDType::ASCII),      2,  0,      "GPSDestBearingRef"},
    {0x0018, to_u16(enIFDType::RATIONAL),   1,  0,      "GPSDestBearing"},
    {0x0019, to_u16(enIFDType::ASCII),      2,  0,      "GPSDestDistanceRef"},
    {0x001a, to_u16(enIFDType::RATIONAL),   1,  0,      "GPSDestDistance"},
    {0x001b, to_u16(enIFDType::UNDEFINED), 0xffff, 0,   "GPSProcessingMethod"},
    {0x001c, to_u16(enIFDType::UNDEFINED), 0xffff, 0,   "GPSAreaInformation"},
    {0x001d, to_u16(enIFDType::ASCII),     11,  0,      "GPSDateStamp"},
    {0x001e, to_u16(enIFDType::SHORT),      1,  0,      "GPSDifferential"},
    {0x001f, to_u16(enIFDType::RATIONAL),   1,  0,      "GPSHPositioningError"},
    // Exif 3.*
    {0x00fe, to_u16(enIFDType::LONG),       1,  0,      "NewSubfileType"},
    {0x010d, to_u16(enIFDType::UTF8),  0xffff,  0,      "DocumentName"},
    {0x010e, to_u16(enIFDType::UTF8),  0xffff,  0,      "ImageDescription"},
    {0x010f, to_u16(enIFDType::UTF8),  0xffff,  0,      "Make"},
    {0x0110, to_u16(enIFDType::UTF8),  0xffff,  0,      "Model"},
    {0x0131, to_u16(enIFDType::UTF8),  0xffff,  0,      "Software"},
    {0x013b, to_u16(enIFDType::UTF8),  0xffff,  0,      "Artist"},
    {0x4201, to_u16(enIFDType::UTF8),  0xffff,  0,      "CameraOwnerName"},
    {0xa460, to_u16(enIFDType::SHORT),      1,  0,      "CompositeImage"},
    {0xa461, to_u16(enIFDType::LONG),       1,  0,      "SourceImageNumberOfCompositeImage"},
    {0xa461, to_u16(enIFDType::SHORT),      1,  0,      "SourceImageNumberOfCompositeImage"},
    {0xa462, to_u16(enIFDType::UNDEFINED),  1,  0,      "SourceExposureTimesOfCompositeImage"},
    // Dummy data for testing SLONG types
    {0x9999, to_u16(enIFDType::SLONG),      1,  0,      "DummySLongForTest"},
};

