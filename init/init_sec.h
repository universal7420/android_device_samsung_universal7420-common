
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
    G920F
};

device_variant match(std::string bl)
{
    if (bl == "G920S") {
        return G920S;
    } else if (bl == "G920K") {
        return G920K;
    } else if (bl == "G920L") {
        return G920L;
    } else if (bl == "G920P") {
        return G920P;
    } else if (bl == "G920I") {
        return G920I;
    } else if (bl == "G920F") {
        return G920F;
    } else {
        return UNKNOWN;
    }
}
