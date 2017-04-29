
/*
 * Copyright (C) 2016 The CyanogenMod Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

enum device_variant {
    UNKNOWN = -1,
    G920S,
    G920K,
    G920L,
    G920P,
    G920I,
    G920F,
    G925F,
    G925S,
    G925K,
    G925L,
    G925I,
    G925W8,
    G920W8
};

device_variant match(std::string bl)
{
    if (bl.find("G920S") != std::string::npos) {
        return G920S;
    } else if (bl.find("G920K") != std::string::npos) {
        return G920K;
    } else if (bl.find("G920L") != std::string::npos) {
        return G920L;
    } else if (bl.find("G920P") != std::string::npos) {
        return G920P;
    } else if (bl.find("G920I") != std::string::npos) {
        return G920I;
    } else if (bl.find("G920F") != std::string::npos) {
        return G920F;
    } else if (bl.find("G925F") != std::string::npos) {
        return G925F;
    } else if (bl.find("G925S") != std::string::npos) {
        return G925S;
    } else if (bl.find("G925K") != std::string::npos) {
        return G925K;
    } else if (bl.find("G925L") != std::string::npos) {
        return G925L;
    } else if (bl.find("G925I") != std::string::npos) {
        return G925I;
    } else if (bl.find("G925W8") != std::string::npos) {
        return G925W8;
    } else if (bl.find("G920W8") != std::string::npos) {
        return G920W8;
    } else {
        return UNKNOWN;
    }
}
