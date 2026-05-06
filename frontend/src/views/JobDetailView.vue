<template>
  <div class="page">
    <div class="container">
      <button class="btn btn-ghost btn-sm back-btn" @click="$router.back()">
        ← Back
      </button>

      <div v-if="!job" class="empty-state">
        <p>Job not found.</p>
        <RouterLink to="/jobs" class="btn btn-outline">Browse Jobs</RouterLink>
      </div>

      <div v-else class="jd-layout">
        <!-- ── Main ────────────────────────────────────────────────────── -->
        <div class="jd-main">
          <!-- Header card -->
          <div class="card jd-header-card">
            <div class="jd-header-inner">
              <div class="jd-logo">{{ job.company.charAt(0) }}</div>
              <div>
                <h1 class="jd-title">{{ job.title }}</h1>
                <p class="jd-company">{{ job.company }}</p>
              </div>
            </div>
            <div class="jd-meta-row">
              <div class="jd-meta-item">
                <svg width="15" height="15" viewBox="0 0 24 24" fill="none"><path d="M12 2C8.13 2 5 5.13 5 9c0 5.25 7 13 7 13s7-7.75 7-13c0-3.87-3.13-7-7-7z" stroke="currentColor" stroke-width="1.8"/><circle cx="12" cy="9" r="2.5" stroke="currentColor" stroke-width="1.8"/></svg>
                {{ job.location }}
              </div>
              <div class="jd-meta-item">
                <svg width="15" height="15" viewBox="0 0 24 24" fill="none"><rect x="2" y="5" width="20" height="14" rx="2" stroke="currentColor" stroke-width="1.8"/><path d="M2 10h20" stroke="currentColor" stroke-width="1.8"/></svg>
                LKR {{ salary }} / month
              </div>
              <div class="jd-meta-item">
                <svg width="15" height="15" viewBox="0 0 24 24" fill="none"><path d="M4 6h16M4 10h10M4 14h8" stroke="currentColor" stroke-width="1.8" stroke-linecap="round"/></svg>
                {{ job.category }}
              </div>
            </div>
            <div class="jd-tags">
              <span :class="job.isOpen ? 'badge badge-green' : 'badge badge-red'">
                {{ job.isOpen ? 'Open' : 'Closed' }}
              </span>
              <span v-if="job.isRemote" class="badge badge-blue">Remote</span>
            </div>
          </div>

          <!-- Description -->
          <div class="card jd-section">
            <h2>Job Description</h2>
            <p class="jd-desc">{{ job.description }}</p>
          </div>

          <!-- Required skills -->
          <div class="card jd-section">
            <h2>Required Skills</h2>
            <div class="jd-skills">
              <span
                v-for="skill in job.requiredSkills"
                :key="skill"
                class="skill-tag"
                :class="{ matched: isSkillMatched(skill) }"
              >
                <svg v-if="isSkillMatched(skill)" width="10" height="10" viewBox="0 0 24 24" fill="none"><path d="M5 12l5 5L20 7" stroke="var(--c-green)" stroke-width="2.5" stroke-linecap="round"/></svg>
                {{ skill }}
              </span>
            </div>
            <p v-if="auth.isLoggedIn && auth.user?.role === 'applicant'" class="match-note">
              <svg width="12" height="12" viewBox="0 0 24 24" fill="none"><path d="M12 2l3.09 6.26L22 9.27l-5 4.87 1.18 6.88L12 17.77l-6.18 3.25L7 14.14 2 9.27l6.91-1.01L12 2z" fill="var(--c-green)"/></svg>
              Green = skills you already have
            </p>
          </div>
        </div>

        <!-- ── Sidebar ──────────────────────────────────────────────────── -->
        <div class="jd-sidebar">
          <!-- Apply card -->
          <div class="card jd-apply-card">
            <h3>Apply for this role</h3>
            <template v-if="!auth.isLoggedIn">
              <p class="apply-note">Sign in to apply for this job.</p>
              <RouterLink to="/login" class="btn btn-primary btn-lg" style="width:100%;justify-content:center">Sign In to Apply</RouterLink>
            </template>
            <template v-else-if="auth.user?.role === 'employer'">
              <p class="apply-note">Employers cannot apply for jobs.</p>
            </template>
            <template v-else-if="!job.isOpen">
              <p class="apply-note">This position is currently closed.</p>
            </template>
            <template v-else-if="alreadyApplied">
              <div class="applied-badge">
                <svg width="20" height="20" viewBox="0 0 24 24" fill="none"><path d="M5 12l5 5L20 7" stroke="var(--c-green)" stroke-width="2" stroke-linecap="round"/></svg>
                Application Submitted
              </div>
              <p class="apply-note">You applied on {{ applicationDate }}.</p>
            </template>
            <template v-else>
              <p class="apply-note">Your profile will be submitted with this application.</p>
              <button class="btn btn-primary btn-lg" style="width:100%;justify-content:center" @click="apply" :disabled="applying">
                <span v-if="applying" class="spinner"/>
                {{ applying ? 'Submitting…' : 'Apply Now' }}
              </button>
            </template>
          </div>

          <!-- Match score -->
          <div v-if="auth.user?.role === 'applicant' && matchScore !== null" class="card jd-match-card">
            <div class="match-header">
              <span class="match-label">Match Score</span>
              <strong class="match-pct">{{ Math.round(matchScore * 100) }}%</strong>
            </div>
            <div class="match-bar">
              <div class="match-fill" :style="{ width: matchScore * 100 + '%' }"/>
            </div>
            <p class="match-desc">Based on your registered skills</p>
          </div>

          <!-- Similar -->
          <div v-if="similar.length" class="card jd-section">
            <h3 style="margin-bottom:12px">Similar Jobs</h3>
            <div class="similar-list">
              <RouterLink
                v-for="s in similar"
                :key="s.id"
                :to="`/jobs/${s.id}`"
                class="similar-item"
              >
                <strong>{{ s.title }}</strong>
                <span>{{ s.company }}</span>
              </RouterLink>
            </div>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
import { computed, ref } from 'vue'
import { useRoute } from 'vue-router'
import { useAuthStore } from '../stores/auth'
import { useDataStore } from '../stores/data'

const route = useRoute()
const auth = useAuthStore()
const data = useDataStore()

const id = Number(route.params.id)
const job = computed(() => data.getJobById(id))

const salary = computed(() => job.value?.salary.toLocaleString('en-LK', { maximumFractionDigits: 0 }) ?? '')

const alreadyApplied = computed(() =>
  auth.user ? data.hasApplied(auth.user.id, id) : false,
)

const myApplication = computed(() =>
  auth.user ? data.getApplicationsByUser(auth.user.id).find(a => a.jobId === id) : null,
)
const applicationDate = computed(() => myApplication.value?.appliedDate ?? '')

const matchScore = computed(() => {
  if (!auth.user || !job.value) return null
  const skills = auth.user.skills.map(s => s.toLowerCase())
  const required = job.value.requiredSkills.map(s => s.toLowerCase())
  if (!required.length) return null
  return required.filter(s => skills.includes(s)).length / required.length
})

// Fixed: Add fallback '[]' to avoid passing 'undefined' to .map() if auth.user is null
function isSkillMatched(skill: string) {
  return (auth.user?.skills || []).map(s => s.toLowerCase()).includes(skill.toLowerCase())
}

const similar = computed(() =>
  data.jobs
    .filter(j => j.id !== id && j.category === job.value?.category && j.isOpen)
    .slice(0, 3),
)

const applying = ref(false)
async function apply() {
  if (!auth.user || !job.value) return
  applying.value = true
  await new Promise(r => setTimeout(r, 600))
  data.submitApplication({
    userId: auth.user.id,
    jobId: job.value.id,
    status: 'pending',
    appliedDate: new Date().toISOString().split('T')[0],
  })
  applying.value = false
}
</script>

<style scoped>
.back-btn { margin-bottom: 24px; }

.jd-layout {
  display: grid;
  grid-template-columns: 1fr 320px;
  gap: 24px;
  align-items: start;
}
@media (max-width: 800px) {
  .jd-layout { grid-template-columns: 1fr; }
  .jd-sidebar { order: -1; }
}

.jd-main, .jd-sidebar { display: flex; flex-direction: column; gap: 16px; }

.jd-header-card { padding: 24px; display: flex; flex-direction: column; gap: 16px; }
.jd-header-inner { display: flex; align-items: center; gap: 16px; }
.jd-logo {
  width: 56px; height: 56px;
  border-radius: 14px;
  background: linear-gradient(135deg, rgba(79,142,247,.2), rgba(124,90,247,.2));
  border: 1px solid rgba(79,142,247,.2);
  display: grid; place-items: center;
  font-family: var(--font-display);
  font-size: 1.5rem; font-weight: 700;
  color: var(--c-accent);
  flex-shrink: 0;
}
.jd-title { font-size: 1.4rem; margin-bottom: 2px; }
.jd-company { color: var(--c-text2); font-size: .9rem; }

.jd-meta-row { display: flex; flex-wrap: wrap; gap: 16px; }
.jd-meta-item { display: flex; align-items: center; gap: 6px; font-size: .85rem; color: var(--c-text2); }
.jd-tags { display: flex; gap: 8px; }

.jd-section { padding: 24px; }
.jd-section h2 { font-size: 1rem; margin-bottom: 12px; }
.jd-desc { color: var(--c-text2); font-size: .9rem; line-height: 1.7; white-space: pre-wrap; }

.jd-skills { display: flex; flex-wrap: wrap; gap: 8px; }
.skill-tag.matched {
  border-color: rgba(62,207,142,.35);
  background: rgba(62,207,142,.08);
  color: var(--c-green);
}
.match-note { margin-top: 10px; font-size: .78rem; color: var(--c-text3); display: flex; align-items: center; gap: 4px; }

.jd-apply-card { padding: 20px; display: flex; flex-direction: column; gap: 12px; }
.jd-apply-card h3 { font-size: .95rem; }
.apply-note { font-size: .82rem; color: var(--c-text2); }
.applied-badge {
  display: flex; align-items: center; gap: 8px;
  padding: 12px 16px;
  background: rgba(62,207,142,.1);
  border: 1px solid rgba(62,207,142,.2);
  border-radius: var(--radius-sm);
  color: var(--c-green);
  font-weight: 500;
  font-size: .9rem;
}

.jd-match-card { padding: 20px; display: flex; flex-direction: column; gap: 8px; }
.match-header { display: flex; justify-content: space-between; align-items: center; }
.match-label { font-size: .8rem; color: var(--c-text2); }
.match-pct { font-family: var(--font-display); font-size: 1.2rem; color: var(--c-green); }
.match-bar { height: 6px; background: var(--c-surface2); border-radius: 3px; overflow: hidden; }
.match-fill { height: 100%; background: linear-gradient(90deg, var(--c-green), var(--c-accent)); transition: width .6s ease; }
.match-desc { font-size: .75rem; color: var(--c-text3); }

.similar-list { display: flex; flex-direction: column; gap: 2px; }
.similar-item {
  display: flex; flex-direction: column; gap: 2px;
  padding: 10px;
  border-radius: var(--radius-sm);
  transition: background .15s;
}
.similar-item:hover { background: var(--c-surface2); }
.similar-item strong { font-size: .88rem; }
.similar-item span { font-size: .78rem; color: var(--c-text2); }

.spinner {
  width: 14px; height: 14px;
  border: 2px solid rgba(255,255,255,.3);
  border-top-color: #fff;
  border-radius: 50%;
  animation: spin .6s linear infinite;
  display: inline-block;
}
@keyframes spin { to { transform: rotate(360deg); } }
</style>