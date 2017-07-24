/*
 * Face Recognition Performance Test
 *
 * This software is not subject to copyright protection and is in the public domain.
 */

#ifndef NULLIMPLFRPC11_H_
#define NULLIMPLFRPC11_H_

#include "frpc.h"

/*
 * Declare the implementation class of the FRPC VERIF (1:1) Interface
 */
namespace FRPC {
    class NullImplFRPC11 : public FRPC::VerifInterface {
public:

    NullImplFRPC11();
    ~NullImplFRPC11() override;

    ReturnStatus
    initialize(const std::string &configDir) override;

    ReturnStatus
    setGPU(uint8_t gpuNum) override;

    ReturnStatus
    createTemplate(
            const Image &face,
            TemplateRole role,
            std::vector<uint8_t> &templ,
            EyePair &eyeCoordinates) override;

    ReturnStatus
    matchTemplates(
            const std::vector<uint8_t> &verifTemplate,
            const std::vector<uint8_t> &enrollTemplate,
            double &similarity) override;

    static std::shared_ptr<FRPC::VerifInterface>
    getImplementation();

private:
    std::string configDir;
    uint8_t whichGPU;
    int counter;
    // Some other members
};
}

#endif /* NULLIMPLFRPC11_H_ */
