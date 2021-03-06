/*****************************************************************************
     The contents of this file are subject to the Mozilla Public License
     Version 1.1 (the "License"); you may not use this file except in
     compliance with the License. You may obtain a copy of the License at
     http://www.mozilla.org/MPL/

     Software distributed under the License is distributed on an "AS IS"
     basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
     License for the specific language governing rights and limitations
     under the License.

     The Original Code is Copyright (C) by Yahoo! Research.

     The Initial Developer of the Original Code is Shravan Narayanamurthy.

     All Rights Reserved.
******************************************************************************/
/*
 * Paramete.cpp
 *
 *  Created on: 14-May-2010
 *      
 */

#include "Parameter.h"
#include "DocumentReader.h"
#include "DocumentWriter.h"
#include "document.pb.h"

Parameter::Parameter() {
    length = 0;
    sum = 0;
    values = NULL;
}

Parameter::~Parameter() {
    if (values != NULL)
        delete[] values;
}

Parameter::Parameter(const Parameter &param) {
    length = 0;
    sum = 0;
    values = NULL;
    *this = param;
}

void Parameter::initialize_from_values(int length_, const double* values_,
        float sum_) {
    length = length_;
    sum = sum_;
    values = new double[length];
    if (values_ != NULL) {
        memcpy(values, values_, sizeof(double) * length_);
    } else {
        for (int i = 0; i < length; i++)
            values[i] = sum / length;
    }
}

void Parameter::initialize_from_dump(string fname) {
    using namespace LDA;
    parameters* params = new parameters;
    DocumentReader doc_rdr(fname);
    doc_rdr.read(params);
    if (params->has_alphasum()) {
        length = params->alphas_size();
        values = new double[length];
        for (int i = 0; i < length; i++)
            values[i] = params->alphas(i);
        sum = params->alphasum();
    }
    delete params;
}

void Parameter::dump(string fname) {
    DocumentWriter doc_writer(fname);
    parameters params;
    params.set_alphasum(sum);
    for (int i = 0; i < length; i++)
        params.add_alphas(values[i]);
    doc_writer.write(params);
}

Parameter& Parameter::operator=(const Parameter &param) {
    length = param.length;
    sum = param.sum;
    if (values != NULL) {
        delete[] values;
        values = NULL;
    }

    if (length > 0) {
        values = new double[length];
        memcpy(values, param.values, sizeof(double) * length);
    }
    return *this;
}
