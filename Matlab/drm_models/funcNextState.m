function x_next = funcNextState(x,param)
assert(length(param) == 3);
iy = param{1};
DRM = param{2};
u = param{3};
DRM.setAnnAndDecoupledStatesRelatedToOutputs(x,iy);
DRM.evalNextStateRelatedToOutputs(u,iy);
x_next = DRM.getAnnStatesRelatedToOutputs(iy);