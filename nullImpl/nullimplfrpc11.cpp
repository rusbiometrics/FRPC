/*
 * Face Recognition Performance Test
 *
 * This software is not subject to copyright protection and is in the public domain.
 */

#include <cstring>

#include "nullimplfrpc11.h"

using namespace std;
using namespace FRPC;

NullImplFRPC11::NullImplFRPC11() {}

NullImplFRPC11::~NullImplFRPC11() {}

ReturnStatus
NullImplFRPC11::initialize(const std::string &configDir)
{
    this->whichGPU = 0;
    this->counter = 0;
    return ReturnStatus(ReturnCode::Success);
}

ReturnStatus
NullImplFRPC11::setGPU(uint8_t gpuNum)
{
    this->whichGPU = gpuNum;
	return ReturnStatus(ReturnCode::Success);
}

ReturnStatus
NullImplFRPC11::createTemplate(
        const Image &face,
        TemplateRole role,
        std::vector<uint8_t> &templ,
        EyePair &eyeCoordinates)
{
    string blurb{"Let the Force be with you...\n"};

    templ.resize(blurb.size());
    memcpy(templ.data(), blurb.c_str(), blurb.size());
    eyeCoordinates = EyePair(true, true, 0, 0, 0, 0);

    return ReturnStatus(ReturnCode::Success);
}

ReturnStatus
NullImplFRPC11::matchTemplates(
        const std::vector<uint8_t> &verifTemplate,
        const std::vector<uint8_t> &enrollTemplate,
        double &similarity)
{
    similarity = std::rand();
    return ReturnStatus(ReturnCode::Success);
}

std::shared_ptr<VerifInterface>
VerifInterface::getImplementation()
{
    return std::make_shared<NullImplFRPC11>();
}





