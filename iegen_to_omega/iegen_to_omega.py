import copy as cp
import re

keywords = ['exists', 'union', 'intersection', 'complement', 'compose', 'inverse', 'domain', 'range',
            'hull', 'codegen', 'farkas', 'forall', 'given', 'and', 'or', 'not', 'within', 'subsetof',
            'supersetof', 'symbolic']

def to_omega(relation, res_id='D'):
    ufuncs = {}  # self._ufuncs
    trim = re.sub(r'\s+', '', relation)
    pos = trim.find(':')
    iters = trim[2:pos - 1].split(',')
    exists = []
    condstr = trim[pos + 1:].rstrip('}')
    conds = condstr.split('&&')
    symcons = {}
    knowns = []
    result = trim

    # Collect conditions and uninterpreted functions...
    for cond in conds:
        # Collect existential variables, so they are not treated as sym consts...
        pos = cond.find('exists(')
        if pos >= 0:
            evars = cond[pos + 7:cond.find(':', pos + 7)]
            [exists.append(evar) for evar in evars.split(',')]
        items = re.findall(r'[A-Za-z_]+', cond)
        for item in items:
            if item not in iters and item.lower() not in keywords:
                if '%s(' % item in result and item not in ufuncs:
                    ufuncs[item] = {'name': item, 'args': [], 'arity': 1, 'order': len(ufuncs),
                                    'oldname': '', 'oldargs': [], 'oldarity': 0}
                if item not in symcons and item not in ufuncs and item not in exists:
                    symcons[item] = len(symcons)
        if isknown(cond, iters, exists, keywords):
            knowns.append(cond)

    # Separate set string for codegen argument from those for given clause.
    given = ''
    if len(knowns) > 0:
        for known in knowns:
            conds.remove(known)
        result = result.replace(condstr, '&&'.join(conds))
        given = '{[%s]: %s}' % (','.join(iters), '&&'.join(knowns))
    symlist = '%s,' % ','.join(sorted(symcons.keys()))

    # U-funcs are tricky, need to consider the args...
    # 1st Pass: Collect all data on u-funcs.
    newfuncs = {}
    for ufname in ufuncs:
        ufunc = ufuncs[ufname]
        fpos = trim.find(ufname)
        while fpos >= 0:
            fpos += len(ufname)
            lpos = trim.find('(', fpos)
            rpos = trim.find(')', lpos + 1)
            args = trim[lpos + 1:rpos].split(',')
            ufunc['arity'] = len(args)
            for i in range(len(args)):
                arg = args[i]
                if len(ufunc['args']) <= i:  # New arg!
                    ufunc['args'].append(arg)
                elif arg != ufunc['args'][i]:
                    newfunc = cp.deepcopy(ufunc)  # New ufunc!
                    newfunc['oldname'] = ufunc['name']
                    newfunc['name'] = '%s%d' % (ufunc['name'], i + 1)
                    newfunc['oldargs'].append(arg)
                    newfuncs[newfunc['name']] = newfunc
            fpos = trim.find(ufname, rpos + 1)

    # Add newbies...
    for ufname in newfuncs:
        ufuncs[ufname] = newfuncs[ufname]

    # 2nd Pass: To prevent prefix errors, need to ensure leading arg must include first iters...
    for ufunc in ufuncs.values():
        if ufunc['arity'] > 0:
            ufunc['oldarity'] = ufunc['arity']
            newargs = []
            for arg in ufunc['args']:
                if arg in iters:
                    newargs = iters[0:iters.index(arg)]
            ufunc['arity'] += len(newargs)
            if len(ufunc['oldargs']) < 1:
                ufunc['oldargs'] = cp.deepcopy(ufunc['args'])
            for it in newargs:
                ufunc['args'].insert(len(ufunc['args']) - 1, it)
        symlist += '%s(%d),' % (ufunc['name'], ufunc['arity'])

    # 3rd Pass: Replace the occurrences of the UFs in the IEGL output...
    for ufunc in ufuncs.values():
        ufname = ufunc['name']
        oldcall = ''
        if len(ufunc['oldname']) > 0:
            oldcall = '%s(%s)' % (ufunc['oldname'], ','.join(ufunc['oldargs']))
        elif ufunc['arity'] > ufunc['oldarity']:
            oldcall = '%s(%s)' % (ufname, ','.join(ufunc['oldargs']))
        if len(oldcall) > 0:
            newcall = '%s(%s)' % (ufname, ','.join(ufunc['args']))
            result = result.replace(oldcall, newcall)
            given = given.replace(oldcall, newcall)

    omcode = "symbolic %s;\n" % symlist[0:len(symlist) - 1]
    omcode += "%s := %s;\n" % (res_id, result)
    # omcode += "%s;\n" % res_id
    omcode += "codegen(%s)" % res_id
    if len(given) > 0:
        omcode += ' given %s' % given
    omcode += ';'

    write_macros(ufuncs)

    return omcode

def isknown(cond, iters, exists, keywords):
    # TODO: Simplify this logic!!!
    isknown = False
    for operand in re.split(r'\>|\<|\=', cond):
        isknown = operand not in iters and operand not in exists
        if not isknown:
            break
    if isknown:
        for keyword in keywords:
            isknown = keyword not in cond
            if not isknown:
                break
    if isknown:
        for operand in re.split(r'\+|\-|\*|\/', cond):
            isknown = operand not in iters and operand not in exists
            if not isknown:
                break
    return isknown

def write_macros(ufuncs):
    out = open('.macros', 'w')
    for ufunc in ufuncs.values():
        arrname = ufunc['name']
        if len(ufunc['oldname']) > 0:
            arrname = ufunc['oldname']
        line = "#define %s(%s) %s[(%s)]\n" % (ufunc['name'],
                ','.join(ufunc['args']), arrname, '),('.join(ufunc['oldargs']))
        out.write(line)
    out.close()

