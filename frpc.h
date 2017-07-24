/*
 * Face Recognition Performance Test
 *
 * This file contains FRPC API description along with the
 * supplied data types definitions and API's interface declaration
 * to be implemented by the face recognition software vendors.
 *
 * This software is not subject to copyright protection
 * and is in the public domain.
 */

#ifndef FRPC_H_
#define FRPC_H_

#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#ifdef BUILD_SHARED_LIBRARY
    #ifdef Q_OS_LINUX
        #define DLLSPEC __attribute__((visibility("default")))
    #else
        #define DLLSPEC __declspec(dllexport)
    #endif
#else
    #ifdef Q_OS_LINUX
        #define DLLSPEC
    #else
        #define DLLSPEC __declspec(dllimport)
    #endif
#endif

namespace FRPC {
/**
 * @brief
 * Struct representing a single image
 */
typedef struct Image {
    /** Number of pixels horizontally */
    uint16_t width;
    /** Number of pixels vertically */
    uint16_t height;
    /** Number of bits per pixel. Legal values are 8 and 24. */
    uint8_t depth;
    /** Managed pointer to raster scanned data.
     * Either RGB color or intensity.
     * If image_depth == 24 this points to  3WH bytes  RGBRGBRGB...
     * If image_depth ==  8 this points to  WH bytes  IIIIIII */
    std::shared_ptr<uint8_t> data;

    Image() :
        width{0},
        height{0},
        depth{24}
        {}

    Image(
        uint16_t width,
        uint16_t height,
        uint8_t depth,
        const std::shared_ptr<uint8_t> &data
        ) :
        width{width},
        height{height},
        depth{depth},
        data{data}
        {}

    /** @brief This function returns the size of the image data. */
    size_t
    size() const { return (width * height * (depth / 8)); }
} Image;


/** Labels describing the type/role of the template
 * to be generated (provided as input to template generation)
 */
enum class TemplateRole {
    /** Enrollment template for 1:1 */
    Enrollment_11,
    /** Verification template for 1:1 */
    Verification_11
};

/**
 * @brief
 * Return codes for functions specified in this API
 */
enum class ReturnCode {
    /** Success */
    Success = 0,
    /** Error reading configuration files */
    ConfigError,
    /** Elective refusal to process the input */
    RefuseInput,
    /** Involuntary failure to process the image */
    ExtractError,
    /** Cannot parse the input data */
    ParseError,
    /** Elective refusal to produce a template */
    TemplateCreationError,
    /** Either or both of the input templates were result of failed
     * feature extraction */
    VerifTemplateError,
    /** The implementation cannot support the number of input images */
    NumDataError,
    /** Template file is an incorrect format or defective */
    TemplateFormatError,
    /**
     * An operation on the enrollment directory
     * failed (e.g. permission, space)
     */
    EnrollDirError,
    /** Cannot locate the input data - the input
    * files or names seem incorrect
    */
    InputLocationError,
    /** There was a problem setting or accessing the GPU */
    GPUError,
    /** Vendor-defined failure */
    VendorError
};

/** Output stream operator for a ReturnCode object. */
inline std::ostream&
operator<<(
    std::ostream &s,
    const ReturnCode &rc)
{
    switch (rc) {
    case ReturnCode::Success:
        return (s << "Success");
    case ReturnCode::ConfigError:
        return (s << "Error reading configuration files");
    case ReturnCode::RefuseInput:
        return (s << "Elective refusal to process the input");
    case ReturnCode::ExtractError:
        return (s << "Involuntary failure to process the image");
    case ReturnCode::ParseError:
        return (s << "Cannot parse the input data");
    case ReturnCode::TemplateCreationError:
        return (s << "Elective refusal to produce a template");
    case ReturnCode::VerifTemplateError:
        return (s << "Either/both input templates were result of failed feature extraction");
    case ReturnCode::NumDataError:
        return (s << "Number of input images not supported");
    case ReturnCode::TemplateFormatError:
        return (s << "Template file is an incorrect format or defective");
    case ReturnCode::EnrollDirError:
        return (s << "An operation on the enrollment directory failed");
    case ReturnCode::InputLocationError:
        return (s << "Cannot locate the input data - the input file or names seem incorrect");
    case ReturnCode::GPUError:
        return (s << "Problem setting or accessing the GPU");
    case ReturnCode::VendorError:
        return (s << "Vendor-defined error");
    default:
        return (s << "Undefined error");
    }
}

/**
 * @brief
 * A structure to contain information about a failure by the software
 * under test.
 *
 * @details
 * An object of this class allows the software to return some information
 * from a function call. The string within this object can be optionally
 * set to provide more information for debugging etc. The status code
 * will be set by the function to Success on success, or one of the
 * other codes on failure.
 */
typedef struct ReturnStatus {
    /** @brief Return status code */
    ReturnCode code;
    /** @brief Optional information string */
    std::string info;

    ReturnStatus() {}
    /**
     * @brief
     * Create a ReturnStatus object.
     *
     * @param[in] code
     * The return status code; required.
     * @param[in] info
     * The optional information string.
     */
    ReturnStatus(
        const ReturnCode code,
        const std::string &info = ""
        ) :
        code{code},
        info{info}
        {}
} ReturnStatus;

typedef struct EyePair
{
    /** If the left eye coordinates have been computed and
     * assigned successfully, this value should be set to true,
     * otherwise false. */
    bool isLeftAssigned;
    /** If the right eye coordinates have been computed and
     * assigned successfully, this value should be set to true,
     * otherwise false. */
    bool isRightAssigned;
    /** X and Y coordinate of the center of the subject's left eye.  If the
     * eye coordinate is out of range (e.g. x < 0 or x >= width), isLeftAssigned
     * should be set to false. */
    uint16_t xleft;
    uint16_t yleft;
    /** X and Y coordinate of the center of the subject's right eye.  If the
     * eye coordinate is out of range (e.g. x < 0 or x >= width), isRightAssigned
     * should be set to false. */
    uint16_t xright;
    uint16_t yright;

    EyePair() :
        isLeftAssigned{false},
        isRightAssigned{false},
        xleft{0},
        yleft{0},
        xright{0},
        yright{0}
        {}

    EyePair(
        bool isLeftAssigned,
        bool isRightAssigned,
        uint16_t xleft,
        uint16_t yleft,
        uint16_t xright,
        uint16_t yright
        ) :
        isLeftAssigned{isLeftAssigned},
        isRightAssigned{isRightAssigned},
        xleft{xleft},
        yleft{yleft},
        xright{xright},
        yright{yright}
        {}
} EyePair;

/**
 * @brief
 * The interface to FRPC Challenge VERIF 1:1 implementation
 *
 * @details
 * The submission software under test will implement this interface by
 * sub-classing this class and implementing each method therein.
 */
class DLLSPEC VerifInterface {
public:
    virtual ~VerifInterface() {}

    /**
     * @brief This function initializes the implementation under test.  It will
     * be called by the SVTest application before any call to createTemplate() or
     * matchTemplates().  The implementation under test should set all parameters.
     * This function will be called N=1 times by the SVTest application.
     *
     * @param[in] configDir
     * A read-only directory containing any developer-supplied configuration
     * parameters or run-time data files.  The name of this directory is
     * assigned by SVTest, not hardwired by the provider.  The names of the
     * files in this directory are hardwired in the implementation and are
     * unrestricted.
     */
    virtual ReturnStatus
    initialize(const std::string &configDir) = 0;

    /**
     * @brief This function takes an Image and outputs a proprietary template
     * and associated eye coordinates.  The vector to store the template will be
     * initially empty, and it is up to the implementation
     * to populate them with the appropriate data.  In all cases, even when unable
     * to extract features, the output shall be a template that may be passed to
     * the match_templates function without error.  That is, this routine must
     * internally encode "template creation failed" and the matcher must
     * transparently handle this.
     *
     * param[in] face
     * Input face image
     * param[in] role
     * Label describing the type/role of the template to be generated
     * param[out] templ
     * The output template.  The format is entirely unregulated.  This will be
     * an empty vector when passed into the function, and the implementation
     * can resize and populate it with the appropriate data.
     * param[out] eyeCoordinates
     * (Optional) The function may choose to return the estimated eye centers
     * for the input face image.
     */
    virtual ReturnStatus
    createTemplate(
        const Image &face,
        TemplateRole role,
        std::vector<uint8_t> &templ,
        EyePair &eyeCoordinates) = 0;

    /**
     * @brief This function compares two proprietary templates and outputs a
     * similarity score, which need not satisfy the metric properties. When
     * either or both of the input templates are the result of a failed
     * template generation, the similarity score shall be -1 and the function
     * return value shall be VerifTemplateError.
     *
     * param[in] verifTemplate
     * A verification template from createTemplate(role=Verification_11).
     * The underlying data can be accessed via verifTemplate.data().  The size,
     * in bytes, of the template could be retrieved as verifTemplate.size().
     * param[in] enrollTemplate
     * An enrollment template from createTemplate(role=Enrollment_11).
     * The underlying data can be accessed via enrollTemplate.data().  The size,
     * in bytes, of the template could be retrieved as enrollTemplate.size().
     * param[out] similarity
     * A similarity score resulting from comparison of the templates,
     * on the range [0,DBL_MAX].
     *
     */
    virtual ReturnStatus
    matchTemplates(
        const std::vector<uint8_t> &verifTemplate,
        const std::vector<uint8_t> &enrollTemplate,
        double &similarity) = 0;

    /**
     * @brief This function sets the GPU device number to be used by all
     * subsequent implementation function calls.  gpuNum is a zero-based
     * sequence value of which GPU device to use.  0 would mean the first
     * detected GPU, 1 would be the second GPU, etc.  If the implementation
     * does not use GPUs, then this function call should simply do nothing.
     *
     * @param[in] gpuNum
     * Index number representing which GPU to use
     */
    virtual ReturnStatus
    setGPU(uint8_t gpuNum) = 0;

    /**
     * @brief
     * Factory method to return a managed pointer to the VerifInterface object.
     * @details
     * This function is implemented by the submitted library and must return
     * a managed pointer to the VerifInterface object.
     *
     * @note
     * A possible implementation might be:
     * return (std::make_shared<Implementation>());
     */
    static std::shared_ptr<VerifInterface>
    getImplementation();
};
/* End of VerifInterface */
}

#endif /* FRPC_H_ */

