int *convcode_extrinsic(double *received, double length, double ***a_priori, t_convcode code, double noise_variance,
                        int decision)
{
    int N_states = 2 << (code.memory - 1);/*{{{*/
    int packet_length = (int) length / code.components - code.memory;

    long int threshold = 1e10;
    // copy a priori probabilities on local array
    double **app = malloc(2 * sizeof(double*));/*{{{*/

    for (int i = 0; i < 2; ++i)
        app[i] = malloc((packet_length + code.memory) * sizeof *app);

    for (int i = 0; i < packet_length; ++i){
        app[0][i] = (*a_priori)[0][i];
        app[1][i] = (*a_priori)[1][i];
    }

    for (int i = 0; i < code.memory; i++) {
        app[0][packet_length + i] = log(0.5);
        app[1][packet_length + i] = log(0.5);
    }

    /*}}}*/

    // initialize backward messages
    double **backward = malloc(N_states * sizeof(double*));/*{{{*/
    for (int k = 0; k < N_states; ++k) {
        backward[k] = malloc((packet_length + code.memory) * sizeof(double));
        backward[k][packet_length + code.memory - 1] = -threshold;
    }

    backward[0][packet_length + code.memory - 1] = 0;

    double *rho = malloc(code.components * sizeof *rho);

    for (int i = packet_length + code.memory - 2; i >= 0; i--) {

        for (int j = 0; j < code.components; ++j)
            rho[j] = received[code.components*(i+1) + j];

        for (int s = 0; s < N_states; ++s) {
            double B = -threshold;

            for (int u = 0; u < 2; ++u) {
                int next = code.next_state[s][u];
                int *out = code.output[s][u];

                double g = 0;
                for (int j = 0; j < code.components; ++j)
                    g += pow(rho[j]- (2*out[j] - 1), 2);

                B = exp_sum(B, app[u][i+1] + backward[next][i+1] + (-g/(2*noise_variance)));
            }

            backward[s][i] = B;
        }

        // normalize
        double max = backward[0][i];
        for (int s = 0; s < N_states; ++s)
            max = backward[s][i] > max ? backward[s][i] : max;

        for (int s = 0; s < N_states; ++s)
            backward[s][i] -= max;
    }/*}}}*/

    // initialize forward messages
    double **forward = malloc(N_states * sizeof(double*));/*{{{*/
    for (int k = 0; k < N_states; ++k) {
        forward[k] = malloc((packet_length + code.memory) * sizeof(double));
        forward[k][0] = -threshold;
    }
    forward[0][0] = 0;


    for (int i = 1; i < packet_length + code.memory; ++i) {

        for (int j = 0; j < code.components; ++j)
            rho[j] = received[code.components*(i-1) + j];

        for (int s = 0; s < N_states; ++s) {

            double F = -threshold;

            // pass through each neighbour
            int *neigh = code.neighbors[s];
            for (int n = 0; n < 2; ++n) {
                int state = abs(neigh[n]) - 1;
                int input = neigh[n] > 0;

                int *out = code.output[state][input];

                double g = 0;
                // compute g
                for (int j = 0; j < code.components; ++j)
                    g += pow(rho[j] - (2*out[j] - 1),2);

                F = exp_sum(F, app[input][i-1] + forward[state][i-1] + (-g/(2*noise_variance)));
            }

            forward[s][i] = F;

        }

        // normalize
        double max = forward[0][i];
        for (int s = 0; s < N_states; ++s)
            max = forward[s][i] > max ? forward[s][i] : max;

        for (int s = 0; s < N_states; ++s)
            forward[s][i] -= max;
    }/*}}}*/

    // initialize extrinsic messages
    double **extrinsic = malloc(2 * sizeof(double*));/*{{{*/

    for (int k = 0; k < 2; ++k) {
        extrinsic[k] = malloc((packet_length * code.memory) * sizeof(double));
    }

    for (int i = 0; i < packet_length + code.memory; ++i) {
        for (int j = 0; j < code.components; ++j)
            rho[j] = received[code.components*i + j];


        for (int u = 0; u < 2; ++u) {
            double E = -threshold;
            for (int s = 0; s < N_states; ++s) {

                int state = code.next_state[s][u];

                double g = 0;

                int *out = code.output[s][u];
                for (int j = 0; j < code.components; ++j)
                    g += pow(rho[j] - (2*out[j] - 1),2);

                double fwd = forward[s][i];
                double bwd = backward[state][i];
                E = exp_sum(E, fwd + bwd + (-g/(2*noise_variance)));
            }

            extrinsic[u][i] = E;
        }

//        double normalization = log(exp(extrinsic[0][i]) + exp(extrinsic[1][i]));
//        extrinsic[0][i] -= normalization;
//        extrinsic[1][i] -= normalization;
        if (i < packet_length)
        {
            (*a_priori)[0][i] = extrinsic[0][i];
            (*a_priori)[1][i] = extrinsic[1][i];
        }
    }/*}}}*/

    // decision
    int *decoded = NULL;
    if (decision){
        decoded = malloc(packet_length * sizeof *decoded);
        for (int i = 0; i < packet_length; ++i) {
            double one = app[1][i] + extrinsic[1][i];
            double zero = app[0][i] + extrinsic[0][i];
            decoded[i] =  one > zero;
        }
    }

    // free memory
    for (int l = 0; l < N_states; ++l) {/*{{{*/
        free(backward[l]);
        free(forward[l]);
    }
    free(backward);
    free(forward);

    for (int i = 0; i < 2; i++) {
        free(extrinsic[i]);
        free(app[i]);
    }
    free(extrinsic);
    free(app);
    free(rho);/*}}}*/

    return decoded;

    /*}}}*/
}

