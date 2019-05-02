int *turbo_decode(double *received, int iterations, double noise_variance, t_turbocode code)
{
    // serial to parallel/*{{{*/
    int *lengths = malloc(2 * sizeof  *lengths);/*{{{*/
    double **streams = malloc(2 * sizeof(double*));
    t_convcode codes[2] = {code.upper_code, code.lower_code};
    for (int i = 0; i < 2; i++) {
        t_convcode cc = codes[i];
        lengths[i] = cc.components * (code.packet_length + cc.memory);
        streams[i] = malloc(lengths[i] * sizeof(double));
    }

    int k = 0, c = 0, cw = 0;
    while (k < code.encoded_length) {
        t_convcode cc = codes[c];

        for (int i = 0; i < cc.components; i++)
           streams[c][cw*cc.components + i] = received[k++];

        c = (c + 1) % 2;
        cw = !c ? cw + 1 : cw;
    }/*}}}*/

    // initial messages
    double **messages = malloc(2 * sizeof(double *));
    for (int i = 0; i < 2; i++) {
        messages[i] = malloc(code.packet_length * sizeof(double));
        for (int j = 0; j < code.packet_length; j++) {
            messages[i][j] = log(0.5);
        }
    }

    int *turbo_decoded = NULL;
    for (int i = 0; i < iterations; i++) {

        // run BCJR on upper code
        convcode_extrinsic(streams[0], lengths[0], &messages, code.upper_code, noise_variance, 0);

        // apply interleaver
        message_interleave(&messages, code);

        // run BCJR on lower code
        turbo_decoded = convcode_extrinsic(streams[1], lengths[1], &messages, code.lower_code, noise_variance, i == (iterations - 1));

        // deinterleave
        message_deinterleave(&messages, code);
    }

    int *decoded_deinterleaved = turbo_deinterleave(turbo_decoded, code);

    for (int i = 0; i < 2; i++)
        free(streams[i]);
    free(streams);
    free(turbo_decoded);

    free(messages[0]);
    free(messages[1]);
    free(messages);

    //length of the 
    return decoded_deinterleaved; /*}}}*/
}
