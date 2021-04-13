//
// Created by Andrew Yang on 4/12/21.
//

#ifndef RAYTRACING_DENOISE_HPP
#define RAYTRACING_DENOISE_HPP

#include <OpenImageDenoise/oidn.hpp>

#include "parallel/params.hpp"

void data_denoise(std::vector<float>& data) {
    std::vector<float> input(params::HEIGHT * params::WIDTH * 3);
    for(int i=0, j=0; i < data.size(); i += 5, j += 3) {
        input[j] = data[i];
        input[j+1] = data[i+1];
        input[j+2] = data[i+2];
    }
    oidn::DeviceRef device = oidn::newDevice();
    device.commit();

    oidn::FilterRef filter = device.newFilter("RT");
    filter.setImage("color", &input[0], oidn::Format::Float3, params::WIDTH, params::HEIGHT);
    filter.setImage("output", &input[0], oidn::Format::Float3, params::WIDTH, params::HEIGHT);
    //filter.set("hdr", true);
    filter.commit();
    filter.execute();

    const char *errorMessage;
    if (device.getError(errorMessage) != oidn::Error::None)
        std::cout << "Error: " << errorMessage << std::endl;

    for(int i=0, j=0; i < data.size(); i += 5, j += 3) {
        data[i] = input[j];
        data[i+1] = input[j+1];
        data[i+2] = input[j+2];
    }
}
#endif //RAYTRACING_DENOISE_HPP
