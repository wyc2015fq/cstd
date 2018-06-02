

//===========================================================================
// Read in the landmark detection validation module
void DetectionValidator::Read(const char* location)
{

  FILE* detection_validator_stream = fopen(location, "rb");

  if (detection_validator_stream) {
    fseek(detection_validator_stream, 0, SEEK_BEG);

    // Read validator type
    fread((char*)&validator_type, 4, 1, detection_validator_stream);

    // Read the number of views (orientations) within the validator
    int n;
    fread((char*)&n, 4, 1, detection_validator_stream);

    orientations.resize(n);
    int i;

    for (i = 0; i < n; i++) {
      Mat orientation_tmp;
      ReadMatBin(detection_validator_stream, orientation_tmp, CV_64F);
      double* ptr = img_ptr(double, &orientation_tmp);
      orientations[i] = Vec3d(ptr[0], ptr[1], ptr[2]);

      // Convert from degrees to radians
      orientations[i] = orientations[i] * (M_PI / 180.0);
    }

    // Initialise the piece-wise affine warps, biases and weights
    paws.resize(n);

    if (validator_type == 0) {
      // Reading in SVRs
      bs.resize(n);
      ws.resize(n);
    }
    else if (validator_type == 1) {
      // Reading in NNs
      ws_nn.resize(n);

      activation_fun.resize(n);
      output_fun.resize(n);
    }
    else if (validator_type == 2) {
      cnn_convolutional_layers.resize(n);
      cnn_convolutional_layers_dft.resize(n);
      cnn_subsampling_layers.resize(n);
      cnn_fully_connected_layers.resize(n);
      cnn_layer_types.resize(n);
      cnn_fully_connected_layers_bias.resize(n);
      cnn_convolutional_layers_bias.resize(n);
    }

    // Initialise the normalisation terms
    mean_images.resize(n);
    standard_deviations.resize(n);

    // Read in the validators for each of the views
    for (i = 0; i < n; i++) {

      // Read in the mean images
      ReadMatBin(detection_validator_stream, mean_images[i], CV_64F);
      mean_images[i] = mean_images[i].t();

      ReadMatBin(detection_validator_stream, standard_deviations[i], CV_64F);
      standard_deviations[i] = standard_deviations[i].t();

      // Model specifics
      if (validator_type == 0) {
        // Reading in the biases and weights
        fread((char*)&bs[i], 8, 1, detection_validator_stream);
        ReadMatBin(detection_validator_stream, ws[i], CV_64F);

      }
      else if (validator_type == 1) {

        // Reading in the number of layers in the neural net
        int num_depth_layers;
        fread((char*)&num_depth_layers, 4, 1, detection_validator_stream);

        // Reading in activation and output function types
        fread((char*)&activation_fun[i], 4, 1, detection_validator_stream);
        fread((char*)&output_fun[i], 4, 1, detection_validator_stream);

        ws_nn[i].resize(num_depth_layers);

        for (int layer = 0; layer < num_depth_layers; layer++) {
          ReadMatBin(detection_validator_stream, ws_nn[i][layer], CV_64F);

          // Transpose for efficiency during multiplication
          ws_nn[i][layer] = ws_nn[i][layer].t();
        }
      }
      else if (validator_type == 2) {
        // Reading in CNNs

        int network_depth;
        fread((char*)&network_depth, 4, 1, detection_validator_stream);

        cnn_layer_types[i].resize(network_depth);

        for (int layer = 0; layer < network_depth; ++layer) {

          int layer_type;
          fread((char*)&layer_type, 4, 1, detection_validator_stream);
          cnn_layer_types[i][layer] = layer_type;

          // convolutional
          if (layer_type == 0) {

            // Read the number of input maps
            int num_in_maps;
            fread((char*)&num_in_maps, 4, 1, detection_validator_stream);

            // Read the number of kernels for each input map
            int num_kernels;
            fread((char*)&num_kernels, 4, 1, detection_validator_stream);

            vector<vector<Mat > > kernels;
            vector<vector<pair<int, Mat > > > kernel_dfts;

            kernels.resize(num_in_maps);
            kernel_dfts.resize(num_in_maps);

            vector<float> biases;

            for (int k = 0; k < num_kernels; ++k) {
              float bias;
              fread((char*)&bias, 4, 1, detection_validator_stream);
              biases.push_back(bias);
            }

            cnn_convolutional_layers_bias[i].push_back(biases);

            // For every input map
            for (int in = 0; in < num_in_maps; ++in) {
              kernels[in].resize(num_kernels);
              kernel_dfts[in].resize(num_kernels);

              // For every kernel on that input map
              for (int k = 0; k < num_kernels; ++k) {
                ReadMatBin(detection_validator_stream, kernels[in][k], CV_32F);

                // Flip the kernel in order to do convolution and not correlation
                flip(kernels[in][k], kernels[in][k], -1);
              }
            }

            cnn_convolutional_layers[i].push_back(kernels);
            cnn_convolutional_layers_dft[i].push_back(kernel_dfts);
          }
          else if (layer_type == 1) {
            // Subsampling layer

            int scale;
            fread((char*)&scale, 4, 1, detection_validator_stream);

            cnn_subsampling_layers[i].push_back(scale);
          }
          else if (layer_type == 2) {
            float bias;
            fread((char*)&bias, 4, 1, detection_validator_stream);
            cnn_fully_connected_layers_bias[i].push_back(bias);

            // Fully connected layer
            Mat weights;
            ReadMatBin(detection_validator_stream, weights, CV_32F);
            cnn_fully_connected_layers[i].push_back(weights);
          }
        }
      }

      // Read in the piece-wise affine warps
      paws[i].Read(detection_validator_stream);
    }
    fclose(detection_validator_stream);
  }
  else {
    printf("WARNING: Can't find the Face checker location\n");
  }
}

//===========================================================================
// Check if the fitting actually succeeded
double DetectionValidator::Check(const Vec3d& orientation, const Mat& intensity_img, Mat& detected_landmarks)
{

  int id = GetViewId(orientation);

  // The warped (cropped) image, corresponding to a face lying withing the detected lanmarks
  Mat warped;

  // the piece-wise affine image
  Mat intensity_img_double;
  intensity_img.convertTo(intensity_img_double, CV_64F);

  //ofstream("D:/tmp/detected_landmarks1.txt") << detected_landmarks;
  paws[id].Warp(intensity_img_double, warped, detected_landmarks);

  double dec;
  if (validator_type == 0) {
    dec = CheckSVR(warped, id);
  }
  else if (validator_type == 1) {
    dec = CheckNN(warped, id);
  }
  else if (validator_type == 2) {
    dec = CheckCNN(warped, id);
  }
  return dec;
}

double DetectionValidator::CheckNN(const Mat& warped_img, int view_id)
{
  Mat feature_vec;
  NormaliseWarpedToVector(warped_img, feature_vec, view_id);
  feature_vec = feature_vec.t();

  for (size_t layer = 0; layer < ws_nn[view_id].size(); ++layer) {
    // Add a bias term
    hconcat(Mat(1, 1, CV_64F, 1.0), feature_vec, feature_vec);

    // Apply the weights
    feature_vec = feature_vec * ws_nn[view_id][layer];

    // Activation or output
    int fun_type;

    if (layer != ws_nn[view_id].size() - 1) {
      fun_type = activation_fun[view_id];
    }
    else {
      fun_type = output_fun[view_id];
    }

    if (fun_type == 0) {
      exp(-feature_vec, feature_vec);
      feature_vec = 1.0 / (1.0 + feature_vec);
    }
    else if (fun_type == 1) {
      double* q1 = img_ptr(double, &feature_vec); // respone for each pixel
      double* q2 = q1 + feature_vec.rows * feature_vec.cols;

      // the logistic function (sigmoid) applied to the response
      while (q1 != q2) {
        *q1 = 1.7159 * tanh((2.0 / 3.0) * (*q1));
        q1++;
      }
    }

    // TODO ReLU

  }

  // Turn it to -1, 1 range
  double dec = (img_ptr(double, &feature_vec)[0] - 0.5) * 2;

  return dec;

}

double DetectionValidator::CheckSVR(const Mat& warped_img, int view_id)
{

  Mat feature_vec;
  NormaliseWarpedToVector(warped_img, feature_vec, view_id);


  double dec = (ws[view_id].dot(feature_vec.t()) + bs[view_id]);

  return dec;

}

// Convolutional Neural Network
double DetectionValidator::CheckCNN(const Mat& warped_img, int view_id)
{
  Mat feature_vec;
  NormaliseWarpedToVector(warped_img, feature_vec, view_id);

  // Create a normalised image from the crop vector
  Mat img(warped_img.rows, warped_img.cols, CV_32F, 0.0);
  img = img.t();

  Mat mask = paws[view_id].pixel_mask.t();
  uchar*  mask_it = img_ptr(uchar, &mask);
  double* feature_it = img_ptr(double, &feature_vec);
  float* img_it = img_ptr(float, &img);
  int wInt = img.cols;
  int hInt = img.rows;

  for (int i = 0; i < wInt; ++i) {
    for (int j = 0; j < hInt; ++j, ++mask_it, ++img_it) {
      // if is within mask
      if (*mask_it) {
        // assign the feature to image if it is within the mask
        *img_it = (float) * feature_it++;
      }
    }
  }

  img = img.t();
  //ofstream("D:/tmp/input_maps1.txt") << img;

  int cnn_layer = 0;
  int subsample_layer = 0;
  int fully_connected_layer = 0;

  vector<Mat > input_maps;
  input_maps.push_back(img);

  vector<Mat > outputs;

  for (size_t layer = 0; layer < cnn_layer_types[view_id].size(); ++layer) {
    // Determine layer type
    int layer_type = cnn_layer_types[view_id][layer];

    // Convolutional layer
    if (layer_type == 0) {
      vector<Mat > outputs_kern;

      for (size_t in = 0; in < input_maps.size(); ++in) {
        Mat input_image = input_maps[in];

        // Useful precomputed data placeholders for quick correlation (convolution)
        Mat input_image_dft;
        Mat integral_image;
        Mat integral_image_sq;

        for (size_t k = 0; k < cnn_convolutional_layers[view_id][cnn_layer][in].size(); ++k) {
          Mat kernel = cnn_convolutional_layers[view_id][cnn_layer][in][k];

          // The convolution (with precomputation)
          Mat output;

          if (cnn_convolutional_layers_dft[view_id][cnn_layer][in][k].second.empty()) {
            std::map<int, Mat > precomputed_dft;

            matchTemplate_m(input_image, input_image_dft, integral_image, integral_image_sq, kernel, precomputed_dft, output, CV_TM_CCORR);

			Mat& aa = precomputed_dft.begin()->second;
            cnn_convolutional_layers_dft[view_id][cnn_layer][in][k].first = precomputed_dft.begin()->first;
            cnn_convolutional_layers_dft[view_id][cnn_layer][in][k].second = precomputed_dft.begin()->second;
          }
          else {
            std::map<int, Mat > precomputed_dft;
            precomputed_dft[cnn_convolutional_layers_dft[view_id][cnn_layer][in][k].first] = cnn_convolutional_layers_dft[view_id][cnn_layer][in][k].second;
            matchTemplate_m(input_image, input_image_dft, integral_image, integral_image_sq, kernel,  precomputed_dft, output, CV_TM_CCORR);
          }

          // Combining the maps
          if (in == 0) {
            outputs_kern.push_back(output);
          }
          else {
            outputs_kern[k] = outputs_kern[k] + output;
          }

        }

      }

      outputs.clear();

      for (size_t k = 0; k < cnn_convolutional_layers[view_id][cnn_layer][0].size(); ++k) {
        // Apply the sigmoid
        exp(-outputs_kern[k] - cnn_convolutional_layers_bias[view_id][cnn_layer][k], outputs_kern[k]);
        outputs_kern[k] = 1.0 / (1.0 + outputs_kern[k]);

        outputs.push_back(outputs_kern[k]);
      }

      cnn_layer++;
    }

    if (layer_type == 1) {
      // Subsampling layer
      int scale = cnn_subsampling_layers[view_id][subsample_layer];

      Mat kx = Mat::ones(2, 1, CC_32F) * 1.0f / scale;
      Mat ky = Mat::ones(1, 2, CC_32F) * 1.0f / scale;

      vector<Mat> outputs_sub;

      for (size_t in = 0; in < input_maps.size(); ++in) {
        Mat conv_out;

        sepFilter2D(input_maps[in], conv_out, CC_32F, kx, ky);
        conv_out = mi(conv_out, cRect(1, 1, conv_out.cols - 1, conv_out.rows - 1));

        int res_rows = conv_out.rows / scale;
        int res_cols = conv_out.cols / scale;

        if (conv_out.rows % scale != 0) {
          res_rows++;
        }

        if (conv_out.cols % scale != 0) {
          res_cols++;
        }

        Mat sub_out(res_rows, res_cols, CV_32F);
        float* sub_out_ptr = img_ptr(float, &sub_out);
        float* conv_out_ptr = img_ptr(float, &conv_out);
        for (int w = 0; w < conv_out.cols; w += scale) {
          for (int h = 0; h < conv_out.rows; h += scale) {
            sub_out_ptr[(h / scale) * res_cols + (w / scale)] = conv_out_ptr[h*conv_out.cols+w];
          }
        }

        outputs_sub.push_back(sub_out);
      }

      outputs = outputs_sub;
      subsample_layer++;

    }

    if (layer_type == 2) {
      // Concatenate all the maps
      Mat input_concat = input_maps[0].t();
      input_concat = input_concat.reshape(0, 1);

      for (size_t in = 1; in < input_maps.size(); ++in) {
        Mat add = input_maps[in].t();
        add = add.reshape(0, 1);
        hconcat(input_concat, add, input_concat);
      }

      input_concat = input_concat * cnn_fully_connected_layers[view_id][fully_connected_layer].t();

      exp(-input_concat - cnn_fully_connected_layers_bias[view_id][fully_connected_layer], input_concat);
      input_concat = 1.0 / (1.0 + input_concat);

      outputs.clear();
      outputs.push_back(input_concat);

      fully_connected_layer++;
    }

    // Set the outputs of this layer to inputs of the next
    input_maps = outputs;

  }

  // Turn it to -1, 1 range
  double dec = (img_ptr(float, &outputs[0])[0] - 0.5) * 2.0;

  return dec;
}

void DetectionValidator::NormaliseWarpedToVector(const Mat& warped_img, Mat& feature_vec, int view_id)
{
  Mat warped_t = warped_img.t();

  // the vector to be filled with paw values
  Mat vec(paws[view_id].number_of_pixels, 1, CV_64F);
  double* vp = img_ptr(double, &vec);
  double* cp = img_ptr(double, &warped_t);

  int wInt = warped_img.cols;
  int hInt = warped_img.rows;

  // the mask indicating if point is within or outside the face region

  Mat maskT = paws[view_id].pixel_mask.t();
  uchar*  mp = img_ptr(uchar, &maskT);

  for (int i = 0; i < wInt; ++i) {
    for (int j = 0; j < hInt; ++j, ++mp, ++cp) {
      // if is within mask
      if (*mp) {
        *vp++ = *cp;
      }
    }
  }

  // Local normalisation
  Scalar mean;
  Scalar std;
  meanStdDev(vec, mean, std);

  // subtract the mean image
  vec -= mean[0];

  // Normalise the image
  if (std[0] == 0) {
    std[0] = 1;
  }

  vec /= std[0];

  // Global normalisation
  feature_vec = (vec - mean_images[view_id])  / standard_deviations[view_id];
}

// Getting the closest view center based on orientation
int DetectionValidator::GetViewId(const Vec3d& orientation) const
{
  int id = 0;

  double dbest = -1.0;

  for (size_t i = 0; i < this->orientations.size(); i++) {

    // Distance to current view
    double d = norm(orientation, this->orientations[i]);

    if (i == 0 || d < dbest) {
      dbest = d;
      id = i;
    }
  }

  return id;

}


