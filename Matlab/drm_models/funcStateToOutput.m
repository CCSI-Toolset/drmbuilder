function y = funcStateToOutput(x,param)
%calculate a subset of D-RM model outputs based on vector of all state variables
assert(length(param) == 2);
iy = param{1};    % output index array
DRM = param{2};   % DRM object
DRM.setAnnStatesRelatedToOutputs(x,iy);
y = DRM.evalRelatedOutputsFromAnnStates(iy);
